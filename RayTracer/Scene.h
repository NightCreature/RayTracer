#pragma once

#include "Shape.h"

#include <filesystem>
#include <vector>

namespace tinyxml2
{
class XMLElement;
}

struct Scene
{
    std::vector<Shape*> m_shapes;
    std::vector<Sphere> m_spheres;
    std::vector<Square> m_squares;
    std::vector<Triangle> m_triangles;

    std::vector<Shape*> m_lights;

    Vector4 TraceRay(Ray ray, size_t bounceCount) const;

    void DeserialiseScene(const std::filesystem::path& file);

    Material ReadMaterialInfo(tinyxml2::XMLElement* xmlElement);

};

