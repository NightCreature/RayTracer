#include "Scene.h"

#include "IntersectionInformation.h"
#include "Math/MathUtilityFunctions.h"
#include "tinyxml2.h"
#include "Utilities.h"

#include <limits>
#include <random>

#include <atomic>
#include "BRDF/BRDF.h"

#undef max

const double bias = 0.000005;

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 Scene::TraceRay(Ray ray, size_t bounceCount) const
{
    IntersectionInformation info;
    bool hit = false;
    bool hitLight = false;
    double lowestIntersectionTime = std::numeric_limits<double>::max();
    Shape* shapeHit = nullptr;
    for (auto shape : m_shapes)
    {
        double intersectionTime = std::numeric_limits<double>::max();
        if (shape->Intersect(ray, intersectionTime))
        {
            if (intersectionTime < lowestIntersectionTime)
            {
                lowestIntersectionTime = intersectionTime;
                info.m_hitPoint = ray.PointAtT(intersectionTime);
                info.m_material = shape->m_material;
                info.m_normal = shape->GetNormalAt(info.m_hitPoint, intersectionTime);
                info.m_objectHit = shape;
                hit = true;
                hitLight = shape->IsLight();

            }
        }
    }

    //This should collide with everything and only repsond to the minimum hit object and cast a ray from there
    if (hit)
    {
        return info.m_material.Shade(info, ray, *this, bounceCount);
    }

    //We hit nothing here so return an environment color
    return Vector4(0.25, 0.25, 0.25, 1);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Scene::DeserialiseScene(const std::filesystem::path& file)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file.string().c_str()) == tinyxml2::XML_NO_ERROR)
    {
        auto xmlNode = doc.FirstChild(); //THis should be the <xml element>
        auto sceneNode = xmlNode->FirstChildElement("scene");
        for (auto xmlElement = sceneNode->FirstChildElement(); xmlElement != nullptr; xmlElement = xmlElement->NextSiblingElement())
        {
            std::string elementName = xmlElement->Value();
            if ("Sphere" == elementName)
            {
                Sphere sphere;
                sphere.m_position = Vector3(xmlElement->DoubleAttribute("x"), xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z"));
                sphere.m_radius = xmlElement->DoubleAttribute("radius");
                sphere.m_isLight = xmlElement->BoolAttribute("isLight");
                sphere.m_material = ReadMaterialInfo(xmlElement);
                sphere.m_center = sphere.m_position;
                m_spheres.push_back(sphere);
            }
            else if (elementName == "Square")
            {
                Square square;
                square.m_position = Vector3(xmlElement->DoubleAttribute("x"), xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z"));
                square.m_normal = Vector3(xmlElement->DoubleAttribute("nx"), xmlElement->DoubleAttribute("ny"), xmlElement->DoubleAttribute("nz"));
                square.m_normal.normalize();
                square.m_size = Vector2(xmlElement->DoubleAttribute("width"), xmlElement->DoubleAttribute("height"));
                square.m_material = ReadMaterialInfo(xmlElement);
                square.m_center = square.m_position;

                m_squares.push_back(square);
            }
            else if (elementName == "Triangle")
            {
                Triangle triangle;
                triangle.m_point1 = Vector3(xmlElement->DoubleAttribute("vx0"), xmlElement->DoubleAttribute("vy0"), xmlElement->DoubleAttribute("vz0"));
                triangle.m_point2 = Vector3(xmlElement->DoubleAttribute("vx1"), xmlElement->DoubleAttribute("vy1"), xmlElement->DoubleAttribute("vz1"));
                triangle.m_point3 = Vector3(xmlElement->DoubleAttribute("vx2"), xmlElement->DoubleAttribute("vy2"), xmlElement->DoubleAttribute("vz2"));

                triangle.m_center = (triangle.m_point1 + triangle.m_point2 + triangle.m_point3) / 3;

                auto v1 = triangle.m_point2 - triangle.m_point1;
                auto v2 = triangle.m_point3 - triangle.m_point1;
                auto normal = cross(v1, v2);
                normal.normalize();
                triangle.m_normal = normal;
                triangle.m_normal = Vector3(xmlElement->DoubleAttribute("nx"), xmlElement->DoubleAttribute("ny"), xmlElement->DoubleAttribute("nz"));
                triangle.m_normal.normalize();
                triangle.m_material = ReadMaterialInfo(xmlElement);

                m_triangles.push_back(triangle);
            }
        }
    }

    for (auto& sphere : m_spheres)
    {
        m_shapes.push_back(&sphere);
        if (sphere.m_isLight)
        {
            m_lights.push_back(&sphere);
        }
    }

    for (auto& square : m_squares)
    {
        m_shapes.push_back(&square);
    }
    
    for (auto& triangle : m_triangles)
    {
        m_shapes.push_back(&triangle);
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Material Scene::ReadMaterialInfo(tinyxml2::XMLElement* xmlElement)
{
    auto diffuseColor = Vector4(xmlElement->DoubleAttribute("r"), xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b"), xmlElement->DoubleAttribute("a"));
    auto type = static_cast<MaterialType>(xmlElement->UnsignedAttribute("materialType"));
    auto refractionIndex = xmlElement->DoubleAttribute("refractionIndex");
    Material material(diffuseColor, diffuseColor, diffuseColor, refractionIndex, type);
    material.AddBRDF(new LambertianBRDF(diffuseColor));
    return material;
}
