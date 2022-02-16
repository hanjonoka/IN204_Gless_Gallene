#pragma once
#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include "utils.h"

class Camera
{
public :
    Vector_t position;
    Vector_t direction;
    float FOV;

public :
    Camera(Vector_t pos, Vector_t dir, int fov) :
        position(pos), direction(dir), FOV(fov)
    {}
};

#endif