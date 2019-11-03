#pragma once

#include "Math/Vector4.h"

struct Material
{
    Vector4 m_diffuseColor;
    double m_refractinIndex = 1.0;
    bool m_isRefelective = false;
    bool m_isRefracting = false;
};

