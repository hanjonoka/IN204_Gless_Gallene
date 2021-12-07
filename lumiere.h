#include "utils.h"
#include "objets.h"
#include <vector>

class Rayon:
{
private:
    Color_t color;
    Vector_t origine;
    Vector_unit_t direction;
public:
    Intersection_t trouve_premier_obstacle(std::vector<Sphere> scene)
    {
        // Trouver un obstacle de la scene qui intersecte, donc dont l'intersection n'est pas NULL, pour avoir une distance
        Intersect_t premier_obstacle = NULL;
        //scene[0].intersect
        //double distance_min = scene[0].intersect(this).

        // Iterer dans la scene pour trouver l'objet dont l'intersection est la plus proche de l'origine du rayon
        for(auto it = std::begin(v); it != std::end(v); ++it) {
            Intersection_t inter = *it.intersect(this);
            if (inter != NULL) {
                if (premier_obstacle == NULL) {premier_obstacle = inter};
                else {
                    if (premier_obstacle.distance > inter.distance) {premier_obstacle = inter;}
                }
            }
        }
        return premier_obstacle;
    }
    void lancer_rayon(Intersection_t inter)
    {
        Rayon diff = Rayon()
    }

}