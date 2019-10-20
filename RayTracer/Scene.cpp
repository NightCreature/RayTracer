#include "Scene.h"

#include "IntersectionInformation.h"
#include <limits>

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 Scene::TraceRay(Ray ray)
{
    for (auto shape : m_shapes)
    {
        IntersectionInformation info;
        if (shape->Intersect(ray, 0.0, std::numeric_limits<double>::max(), info))
        {
            return shape->m_material.m_diffuseColor;
        }
    }
    return Vector4(0, 0, 0, 0);
}
