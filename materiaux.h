#ifndef MAT_H
#define MAT_H

#include "utils.h"

class Material{
    Color_t col_diff;

    float k_diff;
    float k_refl;
    float k_refr;
};

#endif