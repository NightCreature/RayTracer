#include "Shape.h"
#include "IntersectionInformation.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Sphere::Intersect(const Ray& ray, IntersectionInformation& info)
{
    auto originToCenter = m_position - ray.m_origin;
    auto a = ray.m_direction.dot(ray.m_direction);
    auto b = 2 * originToCenter.dot(ray.m_direction);
    auto c = originToCenter.dot(originToCenter) - m_radius * m_radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant >= 0)
    {
        info.m_hitTimeAlongRay = -b - sqrt(discriminant) / (2.0 * a);
        info.m_hitPoint = ray.PointAtT(info.m_hitTimeAlongRay);
        info.m_normal = (m_position - info.m_hitPoint);
        info.m_normal.normalize();
        
    }
    return discriminant > 0;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Triangle::Intersect(const Ray& ray, IntersectionInformation& info)
{
    return false;
}

