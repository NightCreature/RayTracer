#include "Shape.h"
#include "IntersectionInformation.h"

bool solveQuadratic(const double& a, const double& b, const double& c, double& x0, double& x1)
{
    double discr = b * b - 4 * a * c;
    if (discr < 0)
    {
        return false;
    }
    else if (discr == 0)
    {
        x0 = x1 = -0.5 * b / a;
    }
    else
    {
        double q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    
    if (x0 > x1)
    {
        std::swap(x0, x1);
    }

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Sphere::Intersect(const Ray& ray, double& intersectionTime)
{

    double t0, t1; // solutions for t if the ray intersects 

    Vector3 L = ray.m_origin - m_position;
    double a = ray.m_direction.dot(ray.m_direction);
    double b = 2 * ray.m_direction.dot(L);
    double c = L.dot(L) - m_radius * m_radius;
    if (!solveQuadratic(a, b, c, t0, t1)) return false;

    if (t0 > t1)
    {
        std::swap(t0, t1);
    }

    if (t0 < 0)
    {
        t0 = t1; // if t0 is negative, let's use t1 instead 
        if (t0 < 0)
        {
            return false; // both t0 and t1 are negative 
        }
    }

    intersectionTime = t0;

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Triangle::Intersect(const Ray& ray, double& intersectionTime)
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
    {
        return false;
    }
    // At this stage we can compute t to find out where the intersection point is on the line.
    double t = f * edge2.dot(q);
    if (t > EPSILON && t < 1 / EPSILON) // ray intersection
    {
        intersectionTime = t;

        return true;
    }
    else // This means that there is a line intersection but not a ray intersection.
    {
        return false;
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool Square::Intersect(const Ray& ray, double& intersectionTime)
{
    auto denominator = ray.m_direction.dot(m_normal);
    if (denominator > 0) //hit on infinite plane
    {
        intersectionTime = (m_position - ray.m_origin).dot(m_normal) / denominator;

        return true;
    }

    return false;
}
