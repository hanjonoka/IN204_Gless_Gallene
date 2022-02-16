// Ray_tracer.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lumiere.h"
#include "utils.h"
#include "objets.h"
#include "Ray_tracer.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;


#define height 500
#define width 500

std::vector<Sphere_t*>* scene;
std::vector<Sphere_t*>* sources;
Sphere_t* source;
Camera* camera;

Sphere_t* load_json(json data) {
    Sphere_t* sphere = new Sphere_t();
    double x = data["centre"][0];
    double y = data["centre"][1];
    double z = data["centre"][2];
    sphere->centre = Vector_t(x, y, z);
    uint8_t r = data["couleur"][0];
    uint8_t g = data["couleur"][1];
    uint8_t b = data["couleur"][2];
    sphere->couleur = Color_t(r, g, b);
    sphere->radius = data["radius"];
    sphere->source = data["source"];
    return sphere;
}




void init_scene(char* filename)
{
    scene = new std::vector<Sphere_t*>;
    sources = new std::vector<Sphere_t*>;

    camera = new Camera(Vector_t(0,0,0), Vector_t(0,0,1), 50);

    /* Parsing json to create the scene */
    std::ifstream myfile(filename);
    json d;
	if (myfile.is_open()) {
		myfile >> d;
        for (int i = 0; i<(int)(d["Objects"].size()); ++i) {
            Sphere_t* sphere = new Sphere_t();
            sphere->load_json(d["Objects"][i]);
            scene->emplace_back(sphere);
            if (sphere->source) sources->emplace_back(sphere);
        }
        myfile.close();
    }

    // Sphere_t* sphere = new Sphere_t(Vector_t(0,0,10), 3, Color_t(1,0,0), Material::get_gris_mat(),false);
    // std::cout << sphere << "\n";
    // scene->emplace_back(sphere);

    // sphere = new Sphere_t(Vector_t(3,3,6), 0.5, Color_t(1,0,0), Material::get_rouge_mat(),false);
    // std::cout << sphere << "\n";
    // scene->emplace_back(sphere);

    // sphere = new Sphere_t(Vector_t(3,-3,2), 1, Color_t(0,1,0), Material::get_vert_mat(), false);
    // std::cout << sphere << "\n";
    // scene->emplace_back(sphere);

    // sphere = new Sphere_t(Vector_t(-5,-5,5), 1, Color_t(1,1,1), Material(), true);
    // source = sphere;
    // scene->emplace_back(sphere);
    // sources->emplace_back(sphere);

    // sphere = new Sphere_t(Vector_t(5,5,5), 1, Color_t(1,0,0), Material(), true);
    // source = sphere;
    // scene->emplace_back(sphere);
    // sources->emplace_back(sphere);
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
        Vector_t dir = Vector_t(camera->direction + (Vector_t(50/((100.0f-camera->FOV)*(width/2)),0,0)*((i%width)-(width/2))) + (Vector_t(0,50/((100.0f-camera->FOV)*(height/2)),0)*((i/width)-(height/2))));
        // std::cout << dir.u << " " << dir.v << " " << dir.w <<"\n";
        Rayon_t* ray = new Rayon_t(camera->position, dir, scene, sources);

        matrice[i] = ray;

        //correct color so it doesn't overflow
        float exposure = -5.00f;
        ray->color.R = (1.0f - expf(ray->color.R * exposure))*255;
        ray->color.G = (1.0f - expf(ray->color.G * exposure))*255;
        ray->color.B = (1.0f - expf(ray->color.B * exposure))*255;

        image << int(matrice[i]->color.R) << ' ' << int(matrice[i]->color.G) << ' ' << int(matrice[i]->color.B) << "\n";
    }
    image.close();
    return 0;
}

