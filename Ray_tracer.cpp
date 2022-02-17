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


#define height 576
#define width 720

std::vector<Objet_t*>* scene;
std::vector<Objet_t*>* sources;
Sphere_t* source;
Camera* camera;


void init_scene(char* filename)
{
    scene = new std::vector<Objet_t*>;
    sources = new std::vector<Objet_t*>;

    camera = new Camera(Vector_t(0,0,0), Vector_t(0,0,1), 55);
    camera->direction = camera->direction*(1/camera->direction.norme()); 

    /* Parsing json to create the scene */
    std::ifstream myfile(filename);
    json d;
	if (myfile.is_open()) {
		myfile >> d;
        Objet_t* objet;
        for (int i = 0; i<(int)(d["Objects"].size()); ++i) {
            if (d["Objects"][i]["type"]==0) objet = new Sphere_t();
            else if (d["Objects"][i]["type"]==1) objet = new Plan_t();
            else objet = new Rect_t();
            objet->load_json(d["Objects"][i]);
            scene->emplace_back(objet);
            if (objet->source) sources->emplace_back(objet);
        }
        myfile.close();
    }
}

int main(int argc, char *argv[])
{
    if(argc!=2){
        std::cerr << "expected exactly 1 argument : filename\n";
        return 1;
    }
    // Preparation de l'image de retour, format PPM
    std::ofstream image("image.ppm");
    image << "P3\n" << width << " " << height << "\n255\n";

    init_scene(argv[1]);
    std::cout << "init scene !\n";

    Rayon_t* matrice[height*width];
    for (int i=0; i<height*width; i++)
    {
        Vector_t balay = (Vector_t(50/((100.0f-camera->FOV)*(width/2)),0,0)*((i%width)-(width/2))) + (Vector_t(0,50/((100.0f-camera->FOV)*(width/2)),0)*((i/width)-(height/2)));
        Vector_t dir = Vector_t(camera->direction + balay);
        Rayon_t* ray = new Rayon_t(camera->position, dir, scene, sources);

        matrice[i] = ray;

        //correct color so it doesn't overflow
        float exposure = -800.00f;
        ray->color.R = (1.0f - expf(ray->color.R * exposure))*255;
        ray->color.G = (1.0f - expf(ray->color.G * exposure))*255;
        ray->color.B = (1.0f - expf(ray->color.B * exposure))*255;

        image << int(matrice[i]->color.R) << ' ' << int(matrice[i]->color.G) << ' ' << int(matrice[i]->color.B) << "\n";
    }
    image.close();
    return 0;
}