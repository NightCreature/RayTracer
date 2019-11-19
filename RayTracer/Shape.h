#pragma once

#include "Ray.h"
#include "Material.h"

struct IntersectionInformation;

class Shape
{
public:
    virtual bool Intersect(const Ray& ray, double& intersectionTime) = 0;
    virtual Vector3 GetNormalAt(const Vector3& m_hitPoint, double intersectionTime) = 0;

    virtual size_t GetShapeType() = 0;
    virtual bool IsLight() const { return false; }

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
        auto normal = m_hitPoint - m_position;
        normal.normalize();
        return -normal;
    }

    virtual bool IsLight() const override { return m_isLight; }

    virtual size_t GetShapeType() override { return 0; }
    bool m_isLight = false;
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

    virtual size_t GetShapeType() override { return 1; }
};

class Square : public Shape
{
public:
    virtual bool Intersect(const Ray& ray, double& intersectionTime) override;
    virtual Vector3 GetNormalAt(const Vector3& m_hitPoint, double intersectionTime) override
    {
        return m_normal;
    }

    virtual size_t GetShapeType() override { return 2; }

    Vector3 m_position;
    Vector3 m_normal;
    Vector2 m_size;
};