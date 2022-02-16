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
    Intersection_t* intersect;
    std::vector<Sphere_t*>* scene;
    Sphere_t *source_diff;
    std::vector<Sphere_t*>* sources;
    float indice_milieu = 1;

    const static int NB_REBOND_MAX = 10;
    int nb_rebond = 0;

    Rayon_t() : color(Color_t()), origine(Vector_t()), direction(Vector_t()), source_diff(NULL)
    {}

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources)
    {
        init(origine, direction, scene, sources, NULL);
    }

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources, int nb_rebond, Sphere_t* source_diff)
    {
        this->nb_rebond = nb_rebond+1;
        init(origine, direction, scene, sources, source_diff);
    }

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources, int nb_rebond, float indice_milieu)
    {
        this->nb_rebond = nb_rebond+1;
        this->indice_milieu = indice_milieu;
        init(origine, direction, scene, sources, NULL);
    }


    void init(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, std::vector<Sphere_t*>* sources, Sphere_t* source_diff){
        this->origine = origine;
        this->direction = direction;
        this->scene = scene;
        this->sources = sources;
        this->source_diff = source_diff;

        this->color = Color_t(128,128,128);
        if(nb_rebond > NB_REBOND_MAX){
            color = Color_t(0,0,0);
            return;
        }

        if(source_diff == NULL){
            intersect = trouve_premier_obstacle(this->scene);
            if (!intersect) {
                color = Color_t(0, 0, 0);
            }
            else if(intersect->object->source){
                color = intersect->object->material.col_diff;
            }
            else{
                lancer_rayons();  //Calcule la couleur du rayon
                //color = Color_t(255,255,255);
            }
        }else{
            //trouver tous les obstacles entre l'origine et la source de lumière
            //appliquer les transparences
            calcul_diff();
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
        return premier_obstacle;
    }


    void lancer_rayons()
    {
        Vector_t point = (this->origine + (this->direction * (this->intersect->distance/this->direction.norme()) ) );
        Vector_t normale = this->intersect->normale;
        Sphere_t* o = intersect->object;

        // Calcul des rayons diffusés
        //Color_t col_dif = // couleur de la source
        Color_t col_dif = Color_t(0,0,0);
        for(auto it = std::begin(*sources); it != std::end(*sources); ++it){
            Sphere_t* s = *it;
            Vector_t dir_diff = s->centre - point;
            float kd = (dir_diff ^ normale) * (1/(dir_diff.norme() * normale.norme()));
            if(kd>0){
                Rayon_t diff = Rayon_t(point, dir_diff, scene, sources, nb_rebond, s);
                col_dif = col_dif + (o->material.col_diff * diff.color * kd * o->material.k_diff);
            }
        }

        //Calcul du rayon réfléchi
        Color_t col_refl = Color_t(0,0,0);
        Vector_t dir_refl = direction - (normale * (direction^ (normale*2)) );
        Rayon_t refl = Rayon_t(point, dir_refl, scene, sources, nb_rebond, this->indice_milieu);
        col_refl = refl.color * o->material.k_refl;

        //TODO : Calcule du rayon réfracté
        Color_t col_refr = Color_t(0,0,0);



        // this->color = Color_t((col_dif.R+refl.color.R)/2, (col_dif.G+refl.color.G)/2, (col_dif.B+refl.color.B)/2);
        // this->color = diff.color;
        this->color = col_dif + col_refl;
    }

    void calcul_diff(){
        std::vector<Intersection_t*> inters;
        float dist_to_s;
        for(auto it = std::begin(*scene); it != std::end(*scene); ++it) {
            Intersection_t* inter = (*it)->calcul_intersection(this->direction, this->origine);
            if (inter != NULL) {
                inters.emplace_back(inter);
                if(inter->object == this->source_diff){
                    dist_to_s = inter->distance;
                }
            }
        }
        this->color = source_diff->couleur;
        for(auto it = std::begin(inters); it != std::end(inters); ++it) {
            if((*it)->distance<dist_to_s && (*it)->object!=this->source_diff){
                this->color = this->color * (*it)->object->material.col_diff * (*it)->object->material.k_refr;
            }
        }
    }
};


#endif