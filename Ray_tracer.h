#pragma once
#include "utils.h"

class Camera
{
public :
    Vector_t position;
    Vector_t direction;
    int FOV;

public :
    Camera(Vector_t pos, Vector_t dir, int fov) :
        position(pos), direction(dir), FOV(fov)
    {}
};