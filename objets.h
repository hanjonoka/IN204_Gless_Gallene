#ifndef OBJETS_H
#define OBJETS_H

#include <iostream>
#include <fstream>
#include <string>
#include "utils.h"
#include "materiaux.h"
//#include "rapidjson/document.h"
//using namespace rapidjson;
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Sphere_t;

class Intersection_t
{
public :

	double distance;
	Vector_t normale;
	Sphere_t* object = NULL;
    
	Intersection_t()
	{
		distance = 0;
		object = NULL;
	}

	Intersection_t(double dist, Vector_t vect, Sphere_t* o)
	{
		distance = dist;
		normale = vect;
		normale *= (1/normale.norme());
        object = o;
	}

};

class Sphere_t
{   
public:    
    Vector_t centre;
    double radius;
    Color_t couleur;
    bool source;
    Material material;

    Sphere_t() : centre(Vector_t()), radius(1), couleur(Color_t(0, 0, 0)), source(false)
    {}

    Sphere_t(Vector_t centre, double radius, Color_t couleur, Material material) :
        centre(centre), radius(radius), couleur(couleur), source(false), material(material)
    {}

    Sphere_t(Vector_t centre, double radius, Color_t couleur, Material material, bool source) :
        centre(centre), radius(radius), couleur(couleur), source(source), material(material)
    {}

    Sphere_t(const Sphere_t &sphere)
    {
        centre = sphere.centre;
        radius = sphere.radius;
        couleur = sphere.couleur;
        source = sphere.source;
        material = sphere.material;
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
	}

    Intersection_t* calcul_intersection(Vector_t direction, Vector_t origine) // Return an object Intersection representing the intersection of the ray with the current object
    {
        // check if pc is not behind the origin of the ray
        Vector_t v = this->centre - origine;  // %%%% VERIFIER QUE LE - FONCTIONNE SUR LES POSITIONS
        Vector_t dir_norm = (direction * (1/direction.norme()));
        double distance = (v ^ direction)/direction.norme();
        if (distance <= 0) {return NULL;}  // %%%% VERIFIER QUE LE PRODUIT SCALAIRE * FONCTIONNE

        // find pc the projection of the center on the ray
        Vector_t pc = origine + (dir_norm * distance); // %%% VERIF QUE DOUBLE * VECTOR FONCTIONNE OU IMPLEMENTER PROJ

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

            Vector_t i1 = origine + ((direction * (1/direction.norme())) * di1);
            Vector_t normale = i1 - this->centre;
            return new Intersection_t(di1, normale, this);
        }
    }
};


#endif