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
            info.m_material = m_material;
            return true;
        }

        if (intersectionMin < plusRoot && plusRoot < intersectionMax)
        {
            info.m_hitTimeAlongRay = plusRoot;
            info.m_hitPoint = ray.PointAtT(plusRoot);
            info.m_normal = (info.m_hitPoint - m_position) / m_radius;
            info.m_material = m_material;
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
    //auto denominator = ray.m_direction.dot(m_normal);
    //if (denominator > 0) //hit on infinite plane
    //{
    //    info.m_hitTimeAlongRay = (m_position - ray.m_origin).dot(m_normal) / denominator;
    //    info.m_hitPoint = ray.PointAtT(info.m_hitTimeAlongRay);


    //    return true;
    //}

    const double EPSILON = 0.0000001;
    Vector3 vertex0 = m_point1;
    Vector3 vertex1 = m_point2;
    Vector3 vertex2 = m_point3;
    Vector3 edge1, edge2, h, s, q;
    double a, f, u, v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = ray.m_direction.cross(edge2);
    a = edge1.dot(h);
    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    f = 1.0 / a;
    s = ray.m_origin - vertex0;
    u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    q = s.cross(edge1);
    v = f * ray.m_direction.dot(q);
    if (v < 0.0 || u + v > 1.0)
        return false;
    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = f * edge2.dot(q);
    if (t > EPSILON && t < 1 / EPSILON) // ray intersection
    {
        info.m_hitPoint = ray.PointAtT(t);
        info.m_hitTimeAlongRay = t;
        info.m_normal = m_normal;
        //m_material.m_isRefelective = true;
        info.m_material = m_material;

        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
        return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Square::Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info)
{
    auto denominator = ray.m_direction.dot(m_normal);
    if (denominator > 0) //hit on infinite plane
    {
        info.m_hitTimeAlongRay = (m_position - ray.m_origin).dot(m_normal) / denominator;
        info.m_hitPoint = ray.PointAtT(info.m_hitTimeAlongRay);
        info.m_normal = m_normal;
        info.m_material = m_material;

        return true;
    }

    return false;
}
