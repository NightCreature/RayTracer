#include "Scene.h"

#include "IntersectionInformation.h"
#include "Math/MathUtilityFunctions.h"
#include "tinyxml2.h"
#include "Utilities.h"

#include <limits>
#include <random>

#undef max

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 Scene::TraceRay(Ray ray, size_t bounceCount)
{
    for (auto shape : m_shapes)
    {
        IntersectionInformation info;
        if (shape->Intersect(ray, 0.00001, std::numeric_limits<double>::max(), info))
        {
            if (bounceCount > 0)
            {
                Vector3 dir;
                Vector4 color = shape->m_material.m_diffuseColor;
                switch (info.m_material.m_type)
                {
                case MaterialType::diffuse:
                    dir = CreateRandomUnitVector();
                    break;
                case MaterialType::reflective:
                    dir = Reflect(ray, info.m_normal);
                    break;
                case MaterialType::refractive:
                    dir = Refract(ray, info.m_normal, info.m_material.m_refractinIndex);
                    break;
                case MaterialType::fresnel:
                {
                    Vector4 reflectionColor;
                    Vector4 refractionColor;
                    double fresnelFactor;
                    Fresnel(ray, info.m_normal, info.m_material.m_refractinIndex, fresnelFactor);
                    bool outSide = ray.m_direction.dot(info.m_normal);
                    if (fresnelFactor < 1)
                    {
                        Ray refractionRay;
                        refractionRay.m_direction = Refract(ray, info.m_normal, info.m_material.m_refractinIndex);
                        refractionRay.m_origin = outSide ? info.m_hitPoint - 0.005 : info.m_hitPoint + 0.005;
                        refractionColor = TraceRay(refractionRay, bounceCount - 1);
                    }
                    Ray reflectionRay;
                    reflectionRay.m_direction = Reflect(ray, info.m_normal);
                    reflectionRay.m_origin = outSide ? info.m_hitPoint - 0.005 : info.m_hitPoint + 0.005;
                    reflectionColor = TraceRay(reflectionRay, bounceCount - 1);

                    color = reflectionColor * fresnelFactor + refractionColor * (1 - fresnelFactor);

                }
                    break;
                default:
                    break;
                }


                Ray bounceRay;
                bounceRay.m_direction = dir;
                bounceRay.m_direction.normalize();
                bounceRay.m_origin = info.m_hitPoint;
                return color + TraceRay(bounceRay, bounceCount - 1);
            }

            return shape->m_material.m_diffuseColor;
        }
    }

    return Vector4(0, 0, 0, 1);
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
                sphere.m_material.m_diffuseColor = Vector4(xmlElement->DoubleAttribute("r"), xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b"), xmlElement->DoubleAttribute("a"));
                sphere.m_material.m_type = static_cast<MaterialType>(xmlElement->UnsignedAttribute("materialType"));
                sphere.m_material.m_refractinIndex = xmlElement->DoubleAttribute("refractionIndex");

                m_spheres.push_back(sphere);
            }
            else if (elementName == "Square")
            {
                Square square;
                square.m_position = Vector3(xmlElement->DoubleAttribute("x"), xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z"));
                square.m_normal = Vector3(xmlElement->DoubleAttribute("nx"), xmlElement->DoubleAttribute("ny"), xmlElement->DoubleAttribute("nz"));
                square.m_normal.normalize();
                square.m_size = Vector2(xmlElement->DoubleAttribute("width"), xmlElement->DoubleAttribute("height"));
                square.m_material.m_diffuseColor = Vector4(xmlElement->DoubleAttribute("r"), xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b"), xmlElement->DoubleAttribute("a"));
                square.m_material.m_type = static_cast<MaterialType>(xmlElement->UnsignedAttribute("materialType"));
                square.m_material.m_refractinIndex = xmlElement->DoubleAttribute("refractionIndex");

                m_squares.push_back(square);
            }
            else if (elementName == "Triangle")
            {
                Triangle triangle;
                triangle.m_point1 = Vector3(xmlElement->DoubleAttribute("vx0"), xmlElement->DoubleAttribute("vy0"), xmlElement->DoubleAttribute("vz0"));
                triangle.m_point2 = Vector3(xmlElement->DoubleAttribute("vx1"), xmlElement->DoubleAttribute("vy1"), xmlElement->DoubleAttribute("vz1"));
                triangle.m_point3 = Vector3(xmlElement->DoubleAttribute("vx2"), xmlElement->DoubleAttribute("vy2"), xmlElement->DoubleAttribute("vz2"));
                triangle.m_normal = Vector3(xmlElement->DoubleAttribute("nx"), xmlElement->DoubleAttribute("ny"), xmlElement->DoubleAttribute("nz"));
                triangle.m_normal.normalize();
                triangle.m_material.m_diffuseColor = Vector4(xmlElement->DoubleAttribute("r"), xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b"), xmlElement->DoubleAttribute("a"));
                triangle.m_material.m_type = static_cast<MaterialType>(xmlElement->UnsignedAttribute("materialType"));
                triangle.m_material.m_refractinIndex = xmlElement->DoubleAttribute("refractionIndex");
                
                m_triangles.push_back(triangle);
            }
        }
    }

    for (auto& sphere : m_spheres)
    {
        m_shapes.push_back(&sphere);
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
