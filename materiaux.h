#ifndef MAT_H
#define MAT_H

#include "utils.h"

class Material{
public:
    Color_t col_diff;

    float k_diff;
    float k_refl;
    float k_refr;
    float k_spec;

    Material(){};

    Material(float k_diff, float k_refl, float k_refr, float  k_spec, Color_t col_dif){
        this->k_diff = k_diff;
        this->k_refl = k_refl;
        this->k_refr = k_refr;
        this->k_spec = k_spec;
        this->col_diff = col_dif;
    }

    static Material get_gris_mat(){
        return Material(0.7,0.3,0, 0.1,Color_t(0.2,0.2,0.2));
    }

    static Material get_vert_mat(){
        return Material(0.7, 0.3, 0, 0.1,Color_t(0.2,1,0.2));
    }

    static Material get_rouge_mat(){
        return Material(0.7,0.3,0, 0.1,Color_t(0.8,0.2,0.2));
    }
};

#endif