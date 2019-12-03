#include "Material.h"
#include "IntersectionInformation.h"
#include "Ray.h"
#include "Scene.h"
#include "BRDF/BRDF.h"
#include "Math/vector4.h"

#include <windows.h>

#undef max

const double bias = 0.000005;

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Material::~Material()
{
    //Let this leak for now, we should have a copy or move constructor to fix this
    //for (size_t counter = 0; counter < m_brdfs.size(); ++counter)
    //{ 
    //    delete m_brdfs[counter]; 
    //}
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 Material::Shade(const IntersectionInformation& hitInfo, const Ray& ray, const Scene& scene, size_t bounce)
{
    Vector4 color;

    if (bounce == 0)
    {
        return Vector4(0.1, 0.1, 0.1, 1.0);
    }
    
    if (hitInfo.m_objectHit->IsLight())
    {
        return m_emissiveColor; //We hit a light so return its color
    }

    Vector3 wo = ray.m_origin - hitInfo.m_hitPoint;
    wo.normalize(); //This should be the inverse of the ray

    //Can we hit a light directly in the scene, if yes complete the path instantly
    IntersectionInformation info;
    auto offsetBias = hitInfo.m_normal * bias;
    Ray lightRay;
    lightRay.m_origin = hitInfo.m_hitPoint - offsetBias;
    double lowestIntersectionTime = std::numeric_limits<double>::max();
    for (auto shape : scene.m_shapes)
    {
        lightRay.m_direction = shape->m_center - hitInfo.m_hitPoint;
        lightRay.m_direction.normalize();
        double intersectionTime = std::numeric_limits<double>::max();
        if (shape->Intersect(lightRay, intersectionTime))
        {
            if (intersectionTime < lowestIntersectionTime)
            {
                lowestIntersectionTime = intersectionTime;
                info.m_hitPoint = ray.PointAtT(intersectionTime);
                info.m_material = shape->m_material;
                info.m_normal = shape->GetNormalAt(info.m_hitPoint, intersectionTime);
                info.m_objectHit = shape;
            }
        }
    }

    if (info.m_objectHit != nullptr && info.m_objectHit->IsLight())
    {
        Vector3 wi;
        //Get the diffuse color bounce from this object and modulate with the light color
        auto diffuseBrdf = GetBRDF(BRDFReflectionType::Diffuse);
        auto diffuseColor = diffuseBrdf != nullptr ? diffuseBrdf->sample(wi, wo, hitInfo) : Vector4();
        auto specularBrdf = GetBRDF(BRDFReflectionType::Specular);
        auto specularColor = specularBrdf != nullptr ? specularBrdf->sample(wi, wo, hitInfo) : Vector4();
        return (info.m_objectHit->m_material.m_emissiveColor * diffuseColor) + specularColor;
    }

    SampleBRDF(color, hitInfo, wo, scene, bounce, BRDFReflectionType::Diffuse);
    SampleBRDF(color, hitInfo, wo, scene, bounce, BRDFReflectionType::Specular);

    return color;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
BRDF* Material::GetBRDF(BRDFReflectionType type)
{
    for (auto* brdf : m_brdfs)
    {
        if (brdf->GetType() == type)
        {
            return brdf;
        }
    }

    return nullptr;
}

//Spectrum<constant::spectrumSamples> PathBRDF::shade(const Intersection& intersection,
//    const Ray& ray,
//    const int bounce,
//    const TracerSpectrum& pathTracer) const {
//
//    //Max iteration allowed
//    if (bounce == 0) {
//
//        return Spectrum<constant::spectrumSamples>(0.0f);
//    }
//
//
//    Model* object = intersection.object;
//    MaterialBRDF* material = static_cast<MaterialBRDF*>(object->material);
//
//    if (object->isLight == true) {
//
//        //Stop iteration. Light reached.
//        return material->le;
//    }
//
//    Vector3D wo = ray.origin - intersection.point;
//    wo.normalize();
//
//    Spectrum<constant::spectrumSamples> L(0.0f);
//
//    bool randomBRDFsampled = false;
//
//    randomBRDFSampling(randomBRDFsampled, L, wo, *material, intersection, pathTracer, bounce, Diffuse);
//    randomBRDFSampling(randomBRDFsampled, L, wo, *material, intersection, pathTracer, bounce, Specular);
//
//    //If a random BRDF has been sampled return SPD L.
//    //This is a consequence of the fact that random and delta dirac BRDF could not be used
//    //at the same time on the same object.
//    if (randomBRDFsampled == true) {
//
//        return L;
//    }
//
//    deltaDiracBRDFSampling(ray, L, *material, intersection, pathTracer, bounce, Reflection);
//    deltaDiracBRDFSampling(ray, L, *material, intersection, pathTracer, bounce, Transmission);
//
//    return L;
//}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 Material::SampleBRDF(Vector4& color, const IntersectionInformation& hitInfo, const Vector3& wo, const Scene& scene, size_t bounce, BRDFReflectionType type)
{
    Vector3 wi;
    Vector4 sampleColor;
    double pdf = 0.0;

    BRDF* brdf = GetBRDF(type);
    if (brdf != nullptr)
    {
        sampleColor = brdf->sample(wi, wo, hitInfo);
        wi.normalize();
        pdf = brdf->pdf(wi, wo, hitInfo);
    }

    Vector4 colorWeight;
    if (pdf > 0.0)
    {
        colorWeight = (sampleColor * fabs(wi.dot(hitInfo.m_normal))) / pdf;
    }
    else
    {
        //The weight of this bounce is zero so it will make the color black in the end
        return color;
    }

    auto offsetBias = hitInfo.m_normal * bias;
    Ray newRay;
    newRay.m_origin = hitInfo.m_hitPoint - offsetBias;
    newRay.m_direction = wi;

    color = color + colorWeight * scene.TraceRay(newRay, bounce - 1);

    return color;
}


//Spectrum<constant::spectrumSamples> PathBRDF::randomBRDFSampling(bool& randomBRDFsampled,
//    Spectrum<constant::spectrumSamples>& L,
//    const Vector3D& wo,
//    const MaterialBRDF& material,
//    const Intersection& intersection,
//    const TracerSpectrum& pathTracer,
//    const int bounce,
//    const BRDFType type) const {
//
//    Vector3D wi(INFINITY, INFINITY, INFINITY);
//
//    //Sample brdf of the give type.
//    Spectrum<constant::spectrumSamples> f = material.samplef(&wi, wo, intersection, type);
//
//    if (wi.x != INFINITY && wi.y != INFINITY && wi.z != INFINITY) {
//
//        //BRDF has been sampled.
//        randomBRDFsampled = true;
//
//        //Normalize wi.
//        wi.normalize();
//
//        //Get pdf of wi sampled from brdf.
//        float pdf = material.pdf(wi, wo, intersection, type);
//
//        Spectrum<constant::spectrumSamples> weight(0.0f);
//
//        //Check pdf to avoid 0/0 division (and NaN generation).
//        //A spectrum composed of NaN values could bend all samples
//        //during sum in Tracer classes.
//        if (pdf > 0.0f) {
//
//            weight = (f * fabsf(wi.dot(intersection.normal))) / pdf;
//        }
//        else {
//
//            //Weight is 0, so all bounces will be useless.
//            return L;
//        }
//
//        //Setup new ray to be traced.
//        Ray newRay(MathUtils::addEpsilon(intersection.point, intersection.normal), wi);
//
//        //Rendering equation with recursive path tracing calculation.
//        L = L + weight * pathTracer.trace(newRay, bounce - 1);
//    }
//
//    return L;
//}