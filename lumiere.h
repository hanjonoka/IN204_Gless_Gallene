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

    Rayon_t() : color(Color_t()), origine(Vector_t()), direction(Vector_t()), diffusion(false)
    {}

    Rayon_t(Color_t color, Vector_t origine, Vector_t direction) :
        color(color), origine(origine), direction(direction), diffusion(false)
    {}

    Rayon_t(Color_t color, Vector_t origine, Vector_t direction, bool diffusion) :
        color(color), origine(origine), direction(direction), diffusion(diffusion)
    {}



    Intersection_t* trouve_premier_obstacle(std::vector<Sphere_t> scene)
    {
        // Trouver un obstacle de la scene qui intersecte, donc dont l'intersection n'est pas NULL, pour avoir une distance
        Intersection_t* premier_obstacle = NULL;
        //scene[0].intersect
        //double distance_min = scene[0].intersect(this).

        // Iterer dans la scene pour trouver l'objet dont l'intersection est la plus proche de l'origine du rayon
        for(auto it = std::begin(scene); it != std::end(scene); ++it) {
            Intersection_t* inter = it->intersect(*this);
            if (inter != NULL) {
                if (premier_obstacle == NULL) {premier_obstacle = inter;}
                else {
                    if (premier_obstacle->distance > inter->distance) {premier_obstacle = inter;}
                }
            }
        }
        if (this->diffusion && !premier_obstacle->object->lumiere) {return NULL;}
        return premier_obstacle;
    }
    void lancer_rayons(Intersection_t inter, Sphere_t source, std::vector<Sphere_t> scene)
    {
        Vector_t point = this->origine + this->direction * (inter.distance/this->direction.norme());

        // Calcul du rayon diffuse
        //Color_t col_dif = // couleur de la source
        Vector_t dir_diff = source.centre - point;
        Rayon_t diff = Rayon_t(source.couleur, point, dir_diff, true);
        Intersection_t* inter = diff.trouve_premier_obstacle(scene);
        if (inter != NULL) {
            
        }

    }

};