#pragma once

#include "BRDF/BRDFConstantsAndDefinitions.h"
#include "Math/Vector4.h"

#include <vector>

class BRDF;
struct IntersectionInformation;
struct Ray;
struct Scene;

enum class MaterialType : size_t
{
    diffuse = 0,
    reflective,
    refractive,
    fresnel
};


class Material
{
public:
    Material() = default;
    Material(const Vector4& diffuseColor, const Vector4& specularColor, const Vector4& emissiveColor, double refractionIndex, MaterialType type)
        : m_diffuseColor(diffuseColor)
        , m_specularColor(specularColor)
        , m_emissiveColor(emissiveColor)
        , m_refractinIndex(refractionIndex)
        , m_type(type)
    {}
    ~Material();

    Vector4 Shade(const IntersectionInformation& hitInfo, const Ray& ray, const Scene& scene, size_t bounce);

    void AddBRDF(BRDF* brdf) { m_brdfs.push_back(brdf); }
    BRDF* GetBRDF(BRDFReflectionType type);

    const Vector4& GetDiffuseColor() { return m_diffuseColor; }
    void SetDiffuseColor(const Vector4& color) { m_diffuseColor = color; }

    const Vector4& GetEmissiveColor() { return m_emissiveColor; }
    void SetEmissiveColor(const Vector4& color) { m_emissiveColor = color; }

    void SetType(MaterialType type) { m_type = type; }
    void SetRefractionIndex(double index) { m_refractinIndex = index; }
private:
    Vector4 SampleBRDF(Vector4& color, const IntersectionInformation& hitInfo, const Vector3& wo, const Scene& scene, size_t bounce, BRDFReflectionType type);

    Vector4 m_diffuseColor;
    Vector4 m_specularColor;
    Vector4 m_emissiveColor;
    double m_refractinIndex = 1.0;
    MaterialType m_type = MaterialType::diffuse;
    std::vector<BRDF*> m_brdfs; //Material can have several assigned to it diffuse and specular for example
};

