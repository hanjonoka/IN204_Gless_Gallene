// Ray_tracer.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <iostream>
#include "lumiere.h"
#include "utils.h"
//#include "objets.h"
#include "Ray_tracer.h"

#define height 100
#define width 100

std::vector<Sphere_t*>* scene;
Sphere_t* source;
Camera camera;


void init_scene()
{
    camera = Camera(Vector_t(0,0,0), Vector_t(1,0,0), 10);
    
    Sphere_t* sphere = new Sphere_t(&Vector_t(10,0,0), 1, &Color_t(255,0,0), false);
    scene->emplace_back(sphere);

    sphere = new Sphere_t(&Vector_t(10,2,0), 1, &Color_t(0,255,0), false);
    scene->emplace_back(sphere);

    sphere = new Sphere_t(&Vector_t(5,5,0), 1, &Color_t(255,255,255), true);
    scene->emplace_back(sphere);
}

int main()
{
    // Preparation de l'image de retour, format PPM
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    init_scene();
    Rayon_t* matrice[height*width];
    for (int i=0; i<height*width; i++)
    {
        Vector_t dir = Vector_t(camera.direction - Vector_t(5,5,0) + Vector_t(0.1,0,0)*(i%width) + Vector_t(0,0.1,0)*(i/width));
        matrice[i] = new Rayon_t(camera.position, dir, scene, source);
        std::cout << matrice[i]->color.R << ' ' << matrice[i]->color.G << ' ' << matrice[i]->color.B << "\n";
    }

    return 0;
}

