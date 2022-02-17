#ifndef OBJETS_H
#define OBJETS_H

#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"
#include "materiaux.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Objet_t;

class Intersection_t
{
public :

	double distance;
	Vector_t normale;
	Objet_t* object = NULL;
    
	Intersection_t()
	{
		distance = 0;
		object = NULL;
	}

	Intersection_t(double dist, Vector_t vect, Objet_t* o)
	{
		distance = dist;
		normale = vect;
		normale *= (1/normale.norme());
        object = o;
	}

};

class Objet_t
{
public:    
    Vector_t centre;
    Color_t couleur;
    bool source;
    Material material;
    bool has_interior;

    Objet_t() : centre(Vector_t()), couleur(Color_t(0, 0, 0)), source(false), material(Material())
    {}

    virtual void load_json(json data)=0;

    virtual Intersection_t* calcul_intersection(Vector_t direction, Vector_t origine)=0;

};

class Sphere_t : public Objet_t
{   
public:    
    float radius;

    Sphere_t() : radius(1)
    {this->has_interior=true;}

    Sphere_t(Vector_t centre, double radius, Color_t couleur, Material material) : radius(radius) {
        this->centre = centre;
        this->couleur = couleur;
        this->source = false;
        this->material = material;
        this->has_interior=true;
    }

    Sphere_t(Vector_t centre, double radius, Color_t couleur, Material material, bool source) : radius(radius) {
        this->centre = centre;
        this->couleur = couleur;
        this->source = source;
        this->material = material;
        this->has_interior=true;
    }

    Sphere_t(const Sphere_t &sphere)
    {
        centre = sphere.centre;
        radius = sphere.radius;
        couleur = sphere.couleur;
        source = sphere.source;
        material = sphere.material;
        this->has_interior=true;
    }

	/* Construct the object with one value from the json file */
	void load_json(json data) {
		double x = data["centre"][0];
		double y = data["centre"][1];
		double z = data["centre"][2];
		centre = Vector_t(x, y, z);
		uint8_t r = data["couleur"][0];
		uint8_t g = data["couleur"][1];
		uint8_t b = data["couleur"][2];
		couleur = Color_t(r, g, b);
		radius = data["radius"];
		source = data["source"];
        if(source){
            material = Material::get_source(couleur);
        }else{
            material = Material::get_material(data["material"]);
        }
	}

    Intersection_t* calcul_intersection(Vector_t direction, Vector_t origine) // Return an object Intersection representing the intersection of the ray with the current object
    {
        // check if pc is not behind the origin of the ray
        Vector_t v = this->centre - origine; 
        Vector_t dir_norm = (direction * (1/direction.norme()));
        double distance = (v ^ direction)/direction.norme();
        if (distance <= 0) {return NULL;} 

        // find pc the projection of the center on the ray
        Vector_t pc = origine + (dir_norm * distance);

        // check the distance d between pc and the center
        double d = (pc - this->centre).norme();
        // if d > r then no intersection
        if (d > this->radius) {return NULL;}
        // if d == r then intersection = pc
        else if (d == this->radius) {
            Vector_t normale = pc - this->centre;
            return new Intersection_t(distance, normale, this);
        }
        // if d < r then 2 intersections, find the closest and its position
        else {
            double c = sqrt(pow(this->radius, 2) - pow(d, 2)); // pythagorean theorem
            double di1 = distance - c;

            Vector_t i1 = origine + (dir_norm * di1);

            Vector_t normale;
            //si on est à l'intérieur de la sphere.
            if((direction ^ (i1 - origine)) <= 0){
                i1 = i1 + (dir_norm * (2 * c));
                di1 += 2*c;
                normale = this->centre-i1;
            }else{
                normale = i1 - this->centre;
            }
            return new Intersection_t(di1, normale, this);
        }
    }
};

class Plan_t : public Objet_t
{
public :
    Vector_t normale;

    Plan_t() : normale(Vector_t(1, 0 , 0))
    {}

    Plan_t(Vector_t centre, Vector_t normale, Color_t couleur, Material material) : normale(normale) {
        this->centre = centre;
        this->couleur = couleur;
        this->source = false;
        this->material = material;
        this->has_interior = false;
    }

    Plan_t(Vector_t centre, Vector_t normale, Color_t couleur, Material material, bool source) : normale(normale) {
        this->centre = centre;
        this->couleur = couleur;
        this->source = source;
        this->material = material;
        this->has_interior = false;
    }

    Plan_t(const Plan_t &plan)
    {
        centre = plan.centre;
        normale = plan.normale;
        couleur = plan.couleur;
        source = plan.source;
        material = plan.material;
        this->has_interior = false;
    }

    /* Construct the object with one value from the json file */
	void load_json(json data) {
		double x = data["centre"][0];
		double y = data["centre"][1];
		double z = data["centre"][2];
		centre = Vector_t(x, y, z);
		uint8_t r = data["couleur"][0];
		uint8_t g = data["couleur"][1];
		uint8_t b = data["couleur"][2];
        couleur = Color_t(r, g, b);
        double u = data["normale"][0];
		double v = data["normale"][1];
		double w = data["normale"][2];
		normale = Vector_t(u, v, w);
		source = data["source"];
        material = Material::get_material(data["material"]);
	}

    /* Adapted from https://askpythonquestions.com/2021/02/25/intersection-point-of-line-and-plane-python/ */
    Intersection_t* calcul_intersection(Vector_t direction, Vector_t origine) // Return an object Intersection representing the intersection of the ray with the current object
    {
        Vector_t normale_normee = normale*(1/normale.norme());
        Vector_t direction_normee = direction*(1/direction.norme());

        double distance = -((origine-centre)^normale_normee)/(direction_normee^normale_normee);

        if(distance<=0) return NULL;
        if((normale_normee^direction_normee) < 0) normale_normee = normale_normee * -1;

        return new Intersection_t(distance+0.1, normale_normee, this);
    }



};

class Rect_t : public Plan_t
{
public:
    Vector_t orientation;
    double width;
    double height;

    Rect_t() : orientation(Vector_t(0, 1, 0))
    {}

    Rect_t(Vector_t centre, Vector_t normale, Vector_t orientation, double width, double height, Color_t couleur, Material material) : orientation(orientation), width(width), height(height) {
        this->centre = centre;
        this->couleur = couleur;
        this->normale = normale;
        this->source = false;
        this->material = material;
        this->has_interior = false;
    }

    Rect_t(Vector_t centre, Vector_t normale, Vector_t orientation, double width, double height, Color_t couleur, Material material, bool source) : orientation(orientation), width(width), height(height) {
        this->centre = centre;
        this->couleur = couleur;
        this->normale = normale;
        this->source = source;
        this->material = material;
        this->has_interior = false;
    }

    Rect_t(const Rect_t &rect)
    {
        centre = rect.centre;
        normale = rect.normale;
        couleur = rect.couleur;
        source = rect.source;
        material = rect.material;
        orientation = rect.orientation;
        width = rect.width;
        height = rect.height;
        this->has_interior = false;
    }

    /* Construct the object with one value from the json file */
	void load_json(json data) {
		double x = data["centre"][0];
		double y = data["centre"][1];
		double z = data["centre"][2];
		centre = Vector_t(x, y, z);
		uint8_t r = data["couleur"][0];
		uint8_t g = data["couleur"][1];
		uint8_t b = data["couleur"][2];
        couleur = Color_t(r, g, b);
        double u = data["normale"][0];
		double v = data["normale"][1];
		double w = data["normale"][2];
		normale = Vector_t(u, v, w);
        double i = data["orientation"][0];
		double j = data["orientation"][1];
		double k = data["orientation"][2];
		orientation = Vector_t(i, j, k);
        width = data["width"];
        height = data["height"];
		source = data["source"];
        material = Material::get_material(data["material"]);
	}

    /* Adapted from https://askpythonquestions.com/2021/02/25/intersection-point-of-line-and-plane-python/ */
    Intersection_t* calcul_intersection(Vector_t direction, Vector_t origine) // Return an object Intersection representing the intersection of the ray with the current object
    {
        Vector_t normale_normee = normale*(1/normale.norme());
        Vector_t direction_normee = direction*(1/direction.norme());

        double distance = -((origine-centre)^normale_normee)/(direction_normee^normale_normee);

        if(distance<=0) return NULL;
        if((normale_normee^direction_normee) < 0) normale_normee = normale_normee * -1;

        Vector_t u = origine + direction * distance - centre;
        Vector_t w = Vector_t(orientation.v*normale.w-orientation.w*normale.v, orientation.w*normale.u-orientation.u*normale.w, orientation.u*normale.v-orientation.v*normale.u);
        
        if ((u^orientation) >= 0 && (u^orientation)<(width*orientation.norme()) && (u^w)>=0 && (u^w)<(height*orientation.norme()*normale.norme())) return new Intersection_t(distance+0.1, normale_normee, this);
        else return NULL;
    }



};

#endif