// Ray_tracer.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <iostream>
#include <fstream>
#include <string>
#include "lumiere.h"
#include "utils.h"
#include "objets.h"
#include "Ray_tracer.h"
//#include "rapidjson/document.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;


#define height 100
#define width 100

std::vector<Sphere_t*>* scene;
std::vector<Sphere_t*>* sources;
Sphere_t* source;
Camera* camera;


void init_scene(char* filename)
{
    scene = new std::vector<Sphere_t*>;

    camera = new Camera(Vector_t(0,0,0), Vector_t(0,0,1), 10);

    /* Parsing json to create the scene */
    Sphere_t* sphere = new Sphere_t();

    std::ifstream myfile(filename);
    json d;
	if (myfile.is_open()) {
		myfile >> d;
        for (int i = 0; i<3/*d["Objects"].size()*/; ++i) {
            json data = d["Objects"][i];
            std::cout << std::setw(4) << d["Objects"][i] << std::endl;
            sphere = new Sphere_t();
            sphere->load_json(data);

            scene->emplace_back(sphere);
        }
    }
		

    /*Sphere_t* sphere = new Sphere_t(Vector_t(0,0,10), 2, Color_t(255,0,0), false);
    std::cout << sphere << "\n";
    scene->emplace_back(sphere);
    Sphere_t* sphere = new Sphere_t();
    sphere->load_json("Sphere1", "scene.json");


    sphere = new Sphere_t(Vector_t(3,-3,2), 1, Color_t(0,255,0), false);
    std::cout << sphere << "\n";
    scene->emplace_back(sphere);

    sphere = new Sphere_t(Vector_t(5,5,5), 1, Color_t(255,0,0), true);
    source = sphere;
    scene->emplace_back(sphere);
    //sources->emplace_back(sphere);*/
}

int main()
{
    // Preparation de l'image de retour, format PPM
    std::ofstream image("image.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    init_scene("scene.json");
    std::cout << "init scene !\n";

    Rayon_t* matrice[height*width];
    for (int i=0; i<height*width; i++)
    {
        std::cout << i << std::endl;
        Vector_t dir = Vector_t(camera->direction + Vector_t(-1,-1,0) + (Vector_t(0.02,0,0)*(i%width)) + (Vector_t(0,0.02,0)*(i/width)));
        // std::cout << dir.u << " " << dir.v << " " << dir.w <<"\n";
        Rayon_t* ray = new Rayon_t(camera->position, dir, scene, source);

        matrice[i] = ray;
        image << int(matrice[i]->color.R) << ' ' << int(matrice[i]->color.G) << ' ' << int(matrice[i]->color.B) << "\n";
    }
    image.close();
    std::cout << "reached the end wtf" << std::endl;
    return 0;
}

