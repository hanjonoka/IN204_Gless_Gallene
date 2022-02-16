#include <iostream>
#include "objets.h"
#include "utils.h"
#include "materiaux.h"

int main () {
    Plan_t* plan = new Plan_t(Vector_t(0, 0, 0), Vector_t(0, 0, 1), Color_t(1, 0, 0), Material::get_vert_mat());
    Intersection_t* inter = plan->calcul_intersection(Vector_t(0, 0, -2), Vector_t(0, 0, 1));
    std::cout << inter->distance << std::endl;

    return 0;
}