#pragma once

#include "Math/MathUtilityFunctions.h"
#include "Math/vector4.h"
#include "RenderOptions.h"

#include <stdint.h>
#include <string>

struct Ray;
class Vector3;

struct Color
{
    uint8_t b, g, r;
};

bool SaveImage(const std::string& fileName, std::vector<Vector4>& pixels, const RenderOptions& renderOptions);

Vector3 CreateRandomUnitVector();
Vector3 CreateHemiSphereRandomVector(const Vector3& normal);
Vector3 Reflect(const Ray& ray, const Vector3& normal);
Vector3 Refract(const Ray& ray, const Vector3& normal, double refractionIndex);
void Fresnel(const Ray& ray, const Vector3& normal, double refractionIndex, double& fresnelFactor);