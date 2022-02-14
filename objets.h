#ifndef OBJETS_H
#define OBJETS_H

#include "utils.h"
#include "lumiere.h"


/*class Vector_t
{
public :
	double u;
	double v;
	double w;

	Vector_t() : u(0), v(0), w(0)
	{}

	Vector_t(double u, double v, double w) : u(u), v(v), w(w)
	{}

	Vector_t(const Vector_t &vect)
	{
		u = vect.u;
		v = vect.v;
		w = vect.w;
	}

	double norme()
	{
		return sqrt(pow(u, 2) + pow(v, 2) + pow(v, 2));
	}

	template<class T>
	Vector_t operator * (const T n)
	{
		(*this) *= n;
		return *this;
	}

	template<class T>
	Vector_t& operator *= (const T n)
	{
		this->u *= n;
		this->v *= n;
		this->w *= n;
		return *this;
	}

	void operator += (const Vector_t vect)
	{
		this->u += vect.u;
		this->v += vect.v;
		this->w += vect.w;
	}

	Vector_t& operator + (const Vector_t vect)
	{
		(*this) += vect;
		return *this;
	}

	void operator -= (const Vector_t vect)
	{
		this->u -= vect.u;
		this->v -= vect.v;
		this->w -= vect.w;
	}

	Vector_t& operator - (const Vector_t vect)
	{
		(*this) -= vect;
		return *this;
	}


	double operator ^ (const Vector_t& droite)
	{
		return (this->u * droite.u) + (this->v * droite.v) + (this->w * droite.w);
	}

	friend bool operator == (const Vector_t &gauche, const Vector_t &droite)
	{
		return (gauche.u == droite.u && gauche.v == droite.v && gauche.w == droite.w);
	}
};*/

/*class Color_t
{
public :
	Color_t() : R(0), G(0), B(0)
	{}

	Color_t(uint8_t red, uint8_t green, uint8_t blue) :
		R(red), G(green), B(blue)
	{}

	uint8_t R;
	uint8_t G;
	uint8_t B;
};*/

class Sphere_t
{   
public:    
    Vector_t* centre;
    double radius;
    Color_t* couleur;
    bool source;

    Sphere_t() : centre(&Vector_t()), radius(1), couleur(&Color_t(0, 0, 0)), source(false)
    {}

    Sphere_t(Vector_t* centre, double radius, Color_t* couleur) :
        centre(centre), radius(radius), couleur(couleur), source(false)
    {}

    Sphere_t(Vector_t* centre, double radius, Color_t* couleur, bool source) :
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
        Vector_t v = *this->centre - ray.origine;  // %%%% VERIFIER QUE LE - FONCTIONNE SUR LES POSITIONS
        double distance = v ^ ray.direction;
        if (distance <= 0) {return NULL;}  // %%%% VERIFIER QUE LE PRODUIT SCALAIRE * FONCTIONNE

        // find pc the projection of the center on the ray
        Vector_t pc = ray.origine + ray.direction * distance; // %%% VERIF QUE DOUBLE * VECTOR FONCTIONNE OU IMPLEMENTER PROJ

        // check the distance d between pc and the center
        double d = (pc - *this->centre).norme();
        // if d > r then no intersection
        if (d > this->radius) {return NULL;}
        // if d == r then intersection = pc
        else if (d == this->radius) {
            Vector_t normale = pc - *this->centre;
            Intersection_t i(distance, normale, *this);
            return &i;
        }
        // if d < r then 2 intersections, find the closest and its position
        else {
            double c = sqrt(pow(this->radius, 2) - pow(d, 2)); // pythagorean theorem
            double di1 = distance - c;

            Vector_t i1 = ray.origine + ray.direction * di1;
            Vector_t normale = i1 - *this->centre;
            Intersection_t i(di1, normale, *this);
            return &i;
        }

    }
};

#endif