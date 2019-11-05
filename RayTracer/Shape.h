#pragma once

#include "Ray.h"
#include "Material.h"

struct IntersectionInformation;

class Shape
{
public:
    virtual bool Intersect(const Ray& ray, double& intersectionTime) = 0;
    virtual Vector3 GetNormalAt(const Vector3& m_hitPoint, double intersectionTime) = 0;

    Material m_material;
};

class Sphere : public Shape
{
public:

    Vector3 m_position;
    double m_radius;

    virtual bool Intersect(const Ray& ray, double& intersectionTime) override;
    virtual Vector3 GetNormalAt(const Vector3& m_hitPoint, double intersectionTime) override
    {
        return (m_hitPoint - m_position) / m_radius;
    }
};

class Triangle : public Shape
{
public:

    Vector3 m_point1;
    Vector3 m_point2;
    Vector3 m_point3;
    Vector3 m_normal;

    virtual bool Intersect(const Ray& ray, double& intersectionTime) override;
    virtual Vector3 GetNormalAt(const Vector3& m_hitPoint, double intersectionTime) override
    {
        return m_normal;
    }
};

class Square : public Shape
{
public:
    virtual bool Intersect(const Ray& ray, double& intersectionTime) override;
    virtual Vector3 GetNormalAt(const Vector3& m_hitPoint, double intersectionTime) override
    {
        return m_normal;
    }

    Vector3 m_position;
    Vector3 m_normal;
    Vector2 m_size;
};