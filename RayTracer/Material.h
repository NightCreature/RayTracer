#pragma once

#include "Math/Vector4.h"

enum class MaterialType : size_t
{
    diffuse = 0,
    reflective,
    refractive,
    fresnel
};

struct Material
{
    Vector4 m_diffuseColor;
    double m_refractinIndex = 1.0;
    MaterialType m_type;
};

