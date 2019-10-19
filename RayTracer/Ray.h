#pragma once

#include "RenderOptions.h"
#include "Math/vector4.h"
#include "Math/vector3.h"

struct Ray
{
    Vector3 m_origin;
    Vector3 m_direction;

    void CreateRay(const RenderOptions& options, size_t x, size_t y);

    const Vector3 PointAtT(double t) const { return m_origin + m_direction * t; }
};