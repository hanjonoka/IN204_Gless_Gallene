#ifndef MAT_H
#define MAT_H

#include "utils.h"

class Material{
public:
    Color_t col_diff;
    Color_t col_spec;

    float k_diff;
    float k_refl;
    float k_refr;
    float k_spec;
    float i_refr;

    Material(){};

    Material(float k_diff, float k_refl, float k_refr, float  k_spec, float i_refr, Color_t col_dif, Color_t col_spec){
        this->k_diff = k_diff;
        this->k_refl = k_refl;
        this->k_refr = k_refr;
        this->k_spec = k_spec;
        this->i_refr = i_refr;

        this->col_diff = col_dif;
        this->col_spec = col_spec;
    }

    static Material get_source(Color_t color){
        return Material(0.0,0.0,0,0,1,color,Color_t(0,0,0));
    }

    static Material get_gris_mat(){
        return Material(0.7,0.3,0,0.1,1,Color_t(0.2,0.2,0.2),Color_t(0.2,0.2,0.2));
    }

    static Material get_vert_mat(){
        return Material(0.7, 0.3, 0, 0.1,1,Color_t(0.2,1,0.2),Color_t(0.1,0.2,0.1));
    }

    static Material get_rouge_mat(){
        return Material(0.7,0.3,0, 0.1,1,Color_t(0.8,0.2,0.2),Color_t(0.2,1,0.1));
    }

    static Material get_bronze(){
        return Material(0.5,0.75,0.0,56,1,Color_t(0.714,0.4284,0.18144),Color_t(0.9,0.8,0.7));
    }

    static Material get_red_glass(){
        return Material(0.1,0.05,0.98,80,1.5,Color_t(1,0.95,0.95),Color_t(1,1,1));
    }

    static Material get_green_glass(){
        return Material(0.1,0.05,0.98,80,1.5,Color_t(0.95,1,0.95),Color_t(1,1,1));
    }

    static Material get_rose_mat(){
        return Material(0.7,0.3,0, 0.1,1,Color_t(0.9,0.2,0.6),Color_t(0.2,1,0.1));
    }

    static Material get_material(int i){
        switch(i){
            case 0:
                return get_gris_mat();
            case 1:
                return get_rouge_mat();
            case 2:
                return get_vert_mat();
            case 3:
                return get_bronze();
            case 4:
                return get_red_glass();
            case 5:
                return get_green_glass();
            case 6:
                return get_rose_mat();
            default :
                return get_gris_mat();
        }
    }
};

#endif