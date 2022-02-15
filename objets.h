#ifndef OBJETS_H
#define OBJETS_H

#include "utils.h"

class Rayon_t;

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
};

class Intersection_t
{
public :

	double distance;
	Vector_t* normale = NULL;
	Sphere_t* object = NULL;
    
	Intersection_t()
	{
		distance = 0;
		object = NULL;
		normale = NULL;
	}

	Intersection_t(double dist, Vector_t vect, Sphere_t* o)
	{
		distance = dist;
		normale = new Vector_t(vect);
		*normale *= (1/normale->norme());
        object = o;
	}

};

#endif