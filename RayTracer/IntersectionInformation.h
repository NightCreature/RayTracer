#pragma once

#include "Math/Vector3.h"
#include "Math/Vector4.h"

struct IntersectionInformation
{
    double m_hitTimeAlongRay;
    Vector3 m_hitPoint;
    Vector3 m_normal;
    Vector4 m_color;
};

