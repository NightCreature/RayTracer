#pragma once

#include "Math/MathUtilityFunctions.h"
#include "Math/vector4.h"
#include "RenderOptions.h"

#include <stdint.h>
#include <string>


struct Color
{
    uint8_t b, g, r;
};

bool SaveImage(const std::string& fileName, std::vector<Vector4>& pixels, const RenderOptions& renderOptions);

Vector3 CreateRandomUnitVector();