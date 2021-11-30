#include "utils.h"
//#include "rayons.h"

class Sphere:
{   
public:    
    Vector_t centre;
    double radius;
    Color_t couleur;

private:
    // using https://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/
    Intersection_t intersect(Rayon ray)
    {
        // check if pc is not behind the origin of the ray
        Vector_t v = this.centre - ray.origin;  // %%%% VERIFIER QUE LE - FONCTIONNE SUR LES POSITIONS
        double distance = v*ray.direction;
        if (distance <= 0) {return NULL;}  // %%%% VERIFIER QUE LE PRODUIT SCALAIRE * FONCTIONNE

        // find pc the projection of the center on the ray
        Vector_t pc = ray.origin + (distance) * ray.direction; // %%% VERIF QUE DOUBLE * VECTOR FONCTIONNE OU IMPLEMENTER PROJ

        // check the distance d between pc and the center
        double d = norm(pc - this.center);
        // if d > r then no intersection
        if (d > this.radius) {return NULL;}
        // if d == r then intersection = pc
        else if (d == this.radius) {
            Vector_unit_t normale = pc - this.center;
            Intersection_t i(distance, normale, this);
            return i;
        }
        // if d < r then 2 intersections, find the closest and its position
        else {
            double c = sqrt(this.radius**2 - d**2); // pythagorean theorem
            double di1 = distance - c;

            Vector_t i1 = ray.origin + di1*ray.direction;
            Vector_unit_t normale = i1 - this.center;
            Intersection_t i(di1, normale, this);
            return i;
        }

    }
}