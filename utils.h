//#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <cstdint>
#include <math.h>

class Color_t
{
public :
	Color_t() : R(0), G(0), B(0)
	{}

	Color_t(uint8_t red, uint8_t green, uint8_t blue) :
		R(red), G(green), B(blue)
	{}
	Color_t(const Color_t& c) :
		R(c.R), G(c.G), B(c.B)
	{}

	uint8_t R;
	uint8_t G;
	uint8_t B;

	Color_t operator * (Color_t col){
		return Color_t(this->R * col.R,this->G * col.G, this->B * col.B);
	}

	template<class T>
	Color_t operator * (const T n)
	{
		return Color_t(n*this->R, n*this->G,n*this->B);
	}

	Color_t operator + (Color_t col){
		return Color_t(this->R + col.R,this->G + col.G, this->B + col.B);
	}

};

class Position_t
{
public :
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
		return sqrt(pow(u, 2) + pow(v, 2) + pow(w, 2));
	}

	template<class T>
	Vector_t operator * (const T n)
	{
		return Vector_t(n*this->u, n*this->v,n*this->w);
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

	Vector_t operator + (const Vector_t vect)
	{
		return Vector_t(this->u+vect.u, this->v+vect.v, this->w+vect.w);
	}

	void operator -= (const Vector_t vect)
	{
		this->u -= vect.u;
		this->v -= vect.v;
		this->w -= vect.w;
	}

	Vector_t operator - (const Vector_t vect)
	{
		return Vector_t(this->u-vect.u, this->v-vect.v, this->w-vect.w);
	}


	double operator ^ (const Vector_t& droite)
	{
		return (this->u * droite.u) + (this->v * droite.v) + (this->w * droite.w);
	}

	friend bool operator == (const Vector_t &gauche, const Vector_t &droite)
	{
		return (gauche.u == droite.u && gauche.v == droite.v && gauche.w == droite.w);
	}
};

class Vector_unit_t : Vector_t
{
public :
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

	Vector_unit_t(Vector_t v)
	{
		Vector_t(v.u, v.v, v.w);
		double n = this->norme();
		*this *= (1 / n);
	}
};



#endif