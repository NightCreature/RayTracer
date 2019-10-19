#pragma once

#include "Math/Vector3.h"

struct IntersectionInformation
{
    double m_hitTimeAlongRay;
    Vector3 m_hitPoint;
    Vector3 m_normal;
};

