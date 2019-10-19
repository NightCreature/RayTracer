#include "Shape.h"
#include "IntersectionInformation.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Sphere::Intersect(const Ray& ray, IntersectionInformation& info)
{
    auto originToCenter = m_position - ray.m_origin;
    auto cosOriginToCenter = originToCenter.dot(ray.m_direction);
    if (cosOriginToCenter > 0) //Might have an intersection
    {
        auto projectionOfCenterOnRay = cosOriginToCenter * ray.m_direction;
        auto length = projectionOfCenterOnRay.length();
        if (length <= m_radius)
        {
            //intersection possible
            if (length == m_radius)
            {
                //Found the point
                info.m_hit = projectionOfCenterOnRay;
            }
            else
            {
                //Need to find p1, closest hit on the ray.
                auto centerToPc = projectionOfCenterOnRay - m_position;
                auto distance = sqrt(m_radius* m_radius - centerToPc.dot(centerToPc)); 
                auto factorOfIntersection = 0.0;
                if (length > distance)
                {
                    factorOfIntersection = (projectionOfCenterOnRay - ray.m_origin).length() - distance;
                }
                else
                {
                    factorOfIntersection = (projectionOfCenterOnRay - ray.m_origin).length() + distance;
                }

                info.m_hit = ray.m_origin + ray.m_direction * factorOfIntersection;
            }
        }

        return true;
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Triangle::Intersect(const Ray& ray, IntersectionInformation& info)
{
    return false;
}

