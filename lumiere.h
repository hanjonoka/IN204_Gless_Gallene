#ifndef LUMIERE_H
#define LUMIERE_H

#include "utils.h"
#include "objets.h"
#include <vector>

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

    Rayon_t() : color(Color_t()), origine(Vector_t()), direction(Vector_t()), diffusion(false)
    {}

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, Sphere_t *source)
    {
        Rayon_t(origine, direction, scene, false);
    }

    Rayon_t(Vector_t origine, Vector_t direction, std::vector<Sphere_t*>* scene, Sphere_t *source, bool diffusion) :
        origine(origine), direction(direction), diffusion(diffusion), source(source)
    {
        intersect = trouve_premier_obstacle(this->scene);
        if (!intersect) {
            color = Color_t(0, 0, 0);
        }else if(intersect->object->source){
            color = Color_t(intersect->object->couleur);
        }else {
            lancer_rayons();  //Calcule la couleur du rayon
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
            Intersection_t* inter = (*it)->intersect(*this);
            if (inter != NULL) {
                if (premier_obstacle == NULL) {premier_obstacle = inter;}
                else {
                    if (premier_obstacle->distance > inter->distance) {premier_obstacle = inter;}
                }
            }
        }
        if (this->diffusion && !premier_obstacle->object->source) {return NULL;}
        return premier_obstacle;
    }


    void lancer_rayons()
    {
        Vector_t point = this->origine + this->direction * (this->intersect->distance/this->direction.norme());

        // Calcul du rayon diffuse
        //Color_t col_dif = // couleur de la source
        Vector_t dir_diff = source->centre - point;
        Rayon_t diff = Rayon_t(point, dir_diff, scene, source, true);

        //Calcul du rayon réfléchi
        Vector_t dir_refl = direction - (*(intersect->normale) * (direction^ (*(intersect->normale)*2)) );
        Rayon_t refl = Rayon_t(point, dir_refl, scene, source);

        //TODO : Calcule du rayon réfracté

        this->color = Color_t((diff.color.R+refl.color.R)/2, (diff.color.G+refl.color.G)/2, (diff.color.B+refl.color.B)/2);
    }

};


#endif