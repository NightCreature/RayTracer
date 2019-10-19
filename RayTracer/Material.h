#pragma once

#include "Math/Vector4.h"

struct Material
{
    Vector4 m_ambientColor;
    Vector4 m_diffuseColor;
    Vector4 m_emissiveColor;
    bool m_isRefelective = false;
};

