// Ray_tracer.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <iostream>
#include <fstream>
#include "lumiere.h"
#include "utils.h"
#include "objets.h"
#include "Ray_tracer.h"

#define height 100
#define width 100

std::vector<Sphere_t*>* scene;
Sphere_t* source;
Camera* camera;


void init_scene()
{
    scene = new std::vector<Sphere_t*>;

    camera = new Camera(Vector_t(0,0,0), Vector_t(0,0,1), 10);

    Sphere_t* sphere = new Sphere_t(Vector_t(0,0,10), 1, Color_t(255,0,0), false);
    scene->emplace_back(sphere);

    sphere = new Sphere_t(Vector_t(0,2,10), 1, Color_t(0,255,0), false);
    scene->emplace_back(sphere);

    sphere = new Sphere_t(Vector_t(5,5,5), 1, Color_t(255,255,255), true);
    scene->emplace_back(sphere);
}

int main()
{
    // Preparation de l'image de retour, format PPM
    std::ofstream image("image.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    init_scene();
    std::cout << "init scene !\n";

    Rayon_t* matrice[height*width];
    for (int i=0; i<height*width; i++)
    {
        Vector_t dir = Vector_t(camera->direction + Vector_t(-1,-1,0) + (Vector_t(0.02,0,0)*(i%width)) + (Vector_t(0,0.02,0)*(i/width)));
        std::cout << dir.u << " " << dir.v << " " << dir.w <<"\n";
        Rayon_t* ray = new Rayon_t(camera->position, dir, scene, source);

        matrice[i] = ray;
        image << int(matrice[i]->color.R) << ' ' << int(matrice[i]->color.G) << ' ' << int(matrice[i]->color.B) << "\n";
    }
    image.close();

    return 0;
}

