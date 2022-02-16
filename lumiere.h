#ifndef LUMIERE_H
#define LUMIERE_H

#include "objets.h"
#include <vector>
#include <iostream>

class Rayon_t
{
public:
    Color_t color;
    Vector_t origine;
    Vector_t direction;
    bool diffusion;
    Intersection_t* intersect;
    std::vector<Sphere_t*>* scene;
    Sphere_t *source;
    std::vector<Sphere_t*>* sources;

    const static int NB_REBOND_MAX = 10;
    int nb_rebond = 0;

    Rayon_t() : color(Color_t()), origine(Vector_t()), direction(Vector_t()), diffusion(false)
    {}

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources)
    {
        init(origine, direction, scene, sources, false);
    }

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources, int nb_rebond, bool diffusion)
    {
        this->nb_rebond = nb_rebond+1;
        init(origine, direction, scene, sources, diffusion);
    }

    void init(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources, bool diffusion){
        this->origine = origine;
        this->direction = direction;
        this->scene = scene;
        this->sources = sources;
        this->diffusion = diffusion;

        this->color = Color_t(128,128,128);
        if(nb_rebond > NB_REBOND_MAX){
            color = Color_t(0,0,0);
            return;
        }

        intersect = trouve_premier_obstacle(this->scene);
        if (!intersect) {
            color = Color_t(0, 0, 0);
        }
        else if(intersect->object->source){
            color = Color_t(intersect->object->couleur);
        }
        else {
            lancer_rayons();  //Calcule la couleur du rayon
            //color = Color_t(255,255,255);
        }
    }

    Intersection_t* trouve_premier_obstacle(std::vector<Sphere_t*>* scene)
    {
        // Trouver un obstacle de la scene qui intersecte, donc dont l'intersection n'est pas NULL, pour avoir une distance
        Intersection_t* premier_obstacle = NULL;
        //scene[0].intersect
        //double distance_min = scene[0].intersect(this).

        // Iterer dans la scene pour trouver l'objet dont l'intersection est la plus proche de l'origine du rayon
        for(auto it = std::begin(*scene); it != std::end(*scene); ++it) {
            Intersection_t* inter = (*it)->calcul_intersection(this->direction, this->origine);


            if (inter != NULL) {
                if (premier_obstacle == NULL) {
                    premier_obstacle = inter;
                } else {
                    if (premier_obstacle->distance > inter->distance) {
                        premier_obstacle = inter;
                    }
                }
            }

        }
        if (this->diffusion && !premier_obstacle->object->source) return NULL;
        return premier_obstacle;
    }


    void lancer_rayons()
    {
        Vector_t point = (this->origine + (this->direction * (this->intersect->distance/this->direction.norme()) ) );
        Vector_t normale = this->intersect->normale;

        // Calcul des rayons diffusés
        //Color_t col_dif = // couleur de la source
        Color_t col_dif = Color_t(0,0,0);
        for(auto it = std::begin(*sources); it != std::end(*sources); ++it){
            Sphere_t* s = *it;
            Sphere_t* o = intersect->object;
            Vector_t dir_diff = s->centre - point;
            float kd = (dir_diff ^ normale) * (1/(dir_diff.norme() * normale.norme()));
            if(kd<0) break;
            Rayon_t diff = Rayon_t(point, dir_diff, scene, sources, nb_rebond, true);
            col_dif = col_dif + (o->material.col_diff * diff.color * kd);
        }

        //Calcul du rayon réfléchi
        Vector_t dir_refl = direction - (normale * (direction^ (normale*2)) );
        Rayon_t refl = Rayon_t(point, dir_refl, scene, sources, nb_rebond, false);

        //TODO : Calcule du rayon réfracté

        // this->color = Color_t((col_dif.R+refl.color.R)/2, (col_dif.G+refl.color.G)/2, (col_dif.B+refl.color.B)/2);
        // this->color = diff.color;
        this->color = col_dif;
    }
};


#endif