#pragma once

#include "Material.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

class Shape;

struct IntersectionInformation
{
    Material m_material;
    Vector3 m_hitPoint;
    Vector3 m_normal;
    double m_hitTimeAlongRay;
    Shape* m_objectHit = nullptr;
};

