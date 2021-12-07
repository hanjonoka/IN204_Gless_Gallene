#include "utils.h"
#include "objets.h"
#include <vector>

class Rayon_t:
{
public:
    Color_t color;
    Vector_t origine;
    Vector_t direction;

    Rayon_t() : color(Color_t()), origine(Vector_t()), direction(Vector_unit_t())
    {}

    Rayon_t(Color_t color, Vector_t origine, Vector_unit_t direction) :
        color(color), origine(origine), direction(direction)
    {}



    Intersection_t trouve_premier_obstacle(std::vector<Sphere_t> scene)
    {
        // Trouver un obstacle de la scene qui intersecte, donc dont l'intersection n'est pas NULL, pour avoir une distance
        Intersection_t* premier_obstacle = NULL;
        //scene[0].intersect
        //double distance_min = scene[0].intersect(this).

        // Iterer dans la scene pour trouver l'objet dont l'intersection est la plus proche de l'origine du rayon
        for(auto it = std::begin(scene); it != std::end(scene); ++it) {
            Intersection_t* inter = it.intersect(*this);
            if (inter != NULL) {
                if (premier_obstacle == NULL) {premier_obstacle = inter;}
                else {
                    if (premier_obstacle->distance > inter->distance) {premier_obstacle = inter;}
                }
            }
        }
        return *premier_obstacle;
    }
    void lancer_rayon(Intersection_t inter)
    {
        Rayon_t diff = Rayon_t()
    }

}