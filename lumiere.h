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

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, Sphere_t *source)
    {
        init(origine, direction, scene, source, false);
    }

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, Sphere_t *source, int nb_rebond, bool diffusion)
    {
        this->nb_rebond = nb_rebond+1;
        init(origine, direction, scene, source, diffusion);
    }

    void init(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, Sphere_t *source, bool diffusion){
        this->origine = origine;
        this->direction = direction;
        this->scene = scene;
        this->source = source;
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


        // Calcul du rayon diffuse
        //Color_t col_dif = // couleur de la source
        Vector_t dir_diff = source->centre - point;
        Rayon_t diff = Rayon_t(point, dir_diff, scene, source, nb_rebond, true);

        //Calcul du rayon réfléchi
        Vector_t dir_refl = direction - (*(intersect->normale) * (direction^ (*(intersect->normale)*2)) );
        Rayon_t refl = Rayon_t(point, dir_refl, scene, source, nb_rebond, false);

        //TODO : Calcule du rayon réfracté

        this->color = Color_t((diff.color.R+refl.color.R)/2, (diff.color.G+refl.color.G)/2, (diff.color.B+refl.color.B)/2);
        // this->color = diff.color;
        // this->color = Color_t(0,255,0);
    }

    static Intersection_t* calcul_intersection(Sphere_t* sphere, Rayon_t* ray) // Return an object Intersection representing the intersection of the ray with the current object
    {
        // check if pc is not behind the origin of the ray
        Vector_t v = sphere->centre - ray->origine;  // %%%% VERIFIER QUE LE - FONCTIONNE SUR LES POSITIONS
        Vector_t dir_norm = (ray->direction * (1/ray->direction.norme()));
        double distance = (v ^ ray->direction)/ray->direction.norme();
        if (distance <= 0) {return NULL;}  // %%%% VERIFIER QUE LE PRODUIT SCALAIRE * FONCTIONNE

        // find pc the projection of the center on the ray
        Vector_t pc = ray->origine + (dir_norm * distance); // %%% VERIF QUE DOUBLE * VECTOR FONCTIONNE OU IMPLEMENTER PROJ

        // check the distance d between pc and the center
        double d = (pc - sphere->centre).norme();
        // if d > r then no intersection
        if (d > sphere->radius) {return NULL;}
        // if d == r then intersection = pc
        else if (d == sphere->radius) {
            Vector_t normale = pc - sphere->centre;
            return new Intersection_t(distance, normale, sphere);
        }
        // if d < r then 2 intersections, find the closest and its position
        else {
            double c = sqrt(pow(sphere->radius, 2) - pow(d, 2)); // pythagorean theorem
            double di1 = distance - c;

            Vector_t i1 = ray->origine + ((ray->direction * (1/ray->direction.norme())) * di1);
            Vector_t normale = i1 - sphere->centre;
            return new Intersection_t(di1, normale, sphere);
        }
    }
};


#endif