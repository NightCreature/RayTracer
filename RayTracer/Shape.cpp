#include "Shape.h"
#include "IntersectionInformation.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Sphere::Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info)
{
    auto originToCenter = ray.m_origin - m_position; //This causes the b term to be negative which in term keeps the points were we collide above zero
    auto a = ray.m_direction.dot(ray.m_direction);
    auto b = 2 * originToCenter.dot(ray.m_direction);
    auto c = originToCenter.dot(originToCenter) - m_radius * m_radius;
    auto discriminant = b * b - 4 * a * c;
    if (discriminant >= 0)
    {
        //If discriminannt is bigger than zero we actually have 2 roots (-b + sqrt(d) )/2a and (-b - sqrt(d))/2a
        auto rootD = sqrt(discriminant);
        auto minusRoot = (-b - rootD) / 2 * a;
        auto plusRoot = (-b + rootD) / 2 * a;

        if (intersectionMin < minusRoot && minusRoot < intersectionMax)
        {
            info.m_hitTimeAlongRay = minusRoot;
            info.m_hitPoint = ray.PointAtT(minusRoot);
            info.m_normal = (info.m_hitPoint - m_position) / m_radius;
            return true;
        }

        if (intersectionMin < plusRoot && plusRoot < intersectionMax)
        {
            info.m_hitTimeAlongRay = plusRoot;
            info.m_hitPoint = ray.PointAtT(plusRoot);
            info.m_normal = (info.m_hitPoint - m_position) / m_radius;
            return true;
        }

    }
    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Triangle::Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info)
{
    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Square::Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info)
{
    return false;
}
