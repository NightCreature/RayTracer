#include "Scene.h"

#include "IntersectionInformation.h"
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
            //We need to fire a bounce ray of this object to see if we hit anything else, for now we are perfect diffuse whcih means we hit with a random vector in the hemisphere of the normal we just hit
            Vector3 dir = CreateRandomUnitVector();

            Ray bounceRay;
            bounceRay.m_direction = (info.m_hitPoint + info.m_normal + dir) - info.m_hitPoint;
            bounceRay.m_direction.normalize();
            bounceRay.m_origin = info.m_hitPoint;
            for (auto additionalHitShape : m_shapes)
            {
                if (additionalHitShape->Intersect(bounceRay, 0.00001, std::numeric_limits<double>::max(), info))
                {
                    return 0.5 * additionalHitShape->m_material.m_diffuseColor;
                }
            }

            return shape->m_material.m_diffuseColor;
        }
    }

    return Vector4();
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

                m_spheres.push_back(sphere);
            }
            else if (elementName == "Square")
            {
                Square square;
                square.m_position = Vector3(xmlElement->DoubleAttribute("x"), xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z"));
                square.m_size = Vector2(xmlElement->DoubleAttribute("width"), xmlElement->DoubleAttribute("height"));
                square.m_material.m_diffuseColor = Vector4(xmlElement->DoubleAttribute("r"), xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b"), xmlElement->DoubleAttribute("a"));

                m_squares.push_back(square);
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
}
