#pragma once

#include "Shape.h"

#include <filesystem>
#include <vector>

struct Scene
{
    std::vector<Shape*> m_shapes;
    std::vector<Sphere> m_spheres;
    std::vector<Square> m_squares;
    std::vector<Triangle> m_triangles;

    Vector4 TraceRay(Ray ray, size_t bounceCount);

    void DeserialiseScene(const std::filesystem::path& file);
};

