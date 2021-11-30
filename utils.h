#pragma once
#include <cstdint>
#include <math.h>

class Color_t
{
	Color_t() : R(0), G(0), B(0)
	{}

	Color_t(uint8_t red, uint8_t green, uint8_t blue) :
		R(red), G(green), B(blue)
	{}

	uint8_t R;
	uint8_t G;
	uint8_t B;
};

class Position_t
{
	Position_t() : x(0), y(0), z(0)
	{}

	Position_t(double x, double y, double z) : x(x), y(y), z(z)
	{}

	double x;
	double y;
	double z;
};

class Vector_t
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

	inline bool operator == (const Vector_t &gauche, const Vector_t &droite)
	{
		return (gauche->u == droite->u && gauche->v == droite->v && gauche->w == droite->w);
	}
};

class Vector_unit_t : Vector_t
{
	Vector_unit_t()
	{
		Vector_t(1, 0, 0);
	}

	Vector_unit_t(double u, double v, double w)
	{
		Vector_t(u, v, w);
		double n = this->norme();
		*this *= (1 / n);
	}
};