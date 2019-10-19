#include "Scene.h"

#include "IntersectionInformation.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 Scene::TraceRay(Ray ray)
{
    for (auto shape : m_shapes)
    {
        IntersectionInformation info;
        if (shape->Intersect(ray, info))
        {
            return Vector4((info.m_normal + Vector3(1, 1, 1)) * 0.5, 0);
        }
    }
    return Vector4(0, 0, 0, 0);
}
