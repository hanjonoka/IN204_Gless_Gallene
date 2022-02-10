#ifndef OBJETS_H
#define OBJETS_H


#include "utils.h"
//#include "rayons.h"

class Sphere_t
{   
public:    
    Vector_t centre;
    double radius;
    Color_t couleur;
    bool source;

    Sphere_t() : centre(Vector_t()), radius(1), couleur(Color_t(0, 0, 0)), source(false)
    {}

    Sphere_t(Vector_t centre, double radius, Color_t couleur) :
        centre(centre), radius(radius), couleur(couleur), source(false)
    {}

    Sphere_t(Vector_t centre, double radius, Color_t couleur, bool source) :
        centre(centre), radius(radius), couleur(couleur), source(source)
    {}

    Sphere_t(const Sphere_t &sphere)
    {
        centre = sphere.centre;
        radius = sphere.radius;
        couleur = sphere.couleur;
        source = sphere.source;
    }

    // using https://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/
    Intersection_t* intersect(Rayon_t ray) // Return an object Intersection representing the intersection of the ray with the current object
    {
        // check if pc is not behind the origin of the ray
        Vector_t v = this->centre - ray.origine;  // %%%% VERIFIER QUE LE - FONCTIONNE SUR LES POSITIONS
        double distance = v ^ ray.direction;
        if (distance <= 0) {return NULL;}  // %%%% VERIFIER QUE LE PRODUIT SCALAIRE * FONCTIONNE

        // find pc the projection of the center on the ray
        Vector_t pc = ray.origine + ray.direction * distance; // %%% VERIF QUE DOUBLE * VECTOR FONCTIONNE OU IMPLEMENTER PROJ

        // check the distance d between pc and the center
        double d = (pc - this->centre).norme();
        // if d > r then no intersection
        if (d > this->radius) {return NULL;}
        // if d == r then intersection = pc
        else if (d == this->radius) {
            Vector_t normale = pc - this->centre;
            Intersection_t i(distance, normale, *this);
            return &i;
        }
        // if d < r then 2 intersections, find the closest and its position
        else {
            double c = sqrt(pow(this->radius, 2) - pow(d, 2)); // pythagorean theorem
            double di1 = distance - c;

            Vector_t i1 = ray.origine + ray.direction * di1;
            Vector_t normale = i1 - this->centre;
            Intersection_t i(di1, normale, *this);
            return &i;
        }

    }
};

#endif