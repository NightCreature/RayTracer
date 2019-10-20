#pragma once

#include "Ray.h"
#include "Material.h"

struct IntersectionInformation;

class Shape
{
public:
    virtual bool Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info) = 0;

    Material m_material;
};

class Sphere : public Shape
{
public:

    Vector3 m_position;
    double m_radius;

    virtual bool Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info) override;

};

class Triangle : public Shape
{
public:

    Vector3 m_point1;
    Vector3 m_point2;
    Vector3 m_point3;
    Vector3 m_normal;

    virtual bool Intersect(const Ray& ray, double intersectionMin, double intersectionMax, IntersectionInformation& info) override;

};

