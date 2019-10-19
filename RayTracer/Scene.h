#pragma once

#include "Shape.h"

#include <vector>

struct Scene
{
    std::vector<Shape*> m_shapes;

    Vector4 TraceRay(Ray ray);
};

