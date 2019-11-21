#pragma once

#include "../BRDF/BRDFConstantsAndDefinitions.h"

#include "Math/Vector3.h"
#include "Math/Vector4.h"

struct IntersectionInformation;


//Used to give the interface of a Bidirectional Reflectance Distribution Function 
class BRDF
{
public:
    virtual Vector4 sample(Vector3& w0, const Vector3& wi, const IntersectionInformation& intersectionInformation) = 0;
    virtual double pdf(const Vector3& wi, const Vector3 w0, const IntersectionInformation& intersectionInfo) = 0;

    BRDFReflectionType GetType() const { return m_type; }

    Vector3 CosineWeightedHemisphereBRDFDirectionSampling(const IntersectionInformation& intersection) const;

    Vector4 m_color; //Normal raytracing
    Vector4 m_weightedColor; //Path Tracing

    BRDFReflectionType m_type;
};

class LambertianBRDF : public BRDF
{
public:
    LambertianBRDF(const Vector4& color);

    virtual Vector4 sample(Vector3& wi, const Vector3& wo, const IntersectionInformation& intersectionInformation) override;
    virtual double pdf(const Vector3& wi, const Vector3 w0, const IntersectionInformation& intersectionInfo) override;
};
