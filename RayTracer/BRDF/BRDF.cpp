#include "BRDF.h"

#include "../IntersectionInformation.h"
#include "Math/matrixmath.h"
#include "Math/vector4.h"
#include "../Utilities.h"

#include <random>



std::random_device srdBRDF;
std::mt19937 sgenBRDF(srdBRDF());
std::uniform_real_distribution<> sdisBRDF(0.0, 1.0);

void generateCoodinateSystem(const Vector3& vector1, Vector3& vector2, Vector3& vector3) 
{

    if (std::fabs(vector1.x()) > std::fabs(vector1.y())) 
    {

        vector2 = Vector3(vector1.z(), 0, -vector1.x());
    }
    else 
    {

        vector2 = Vector3(0, -vector1.z(), vector1.y());
    }

    vector2.normalize();

    vector3 = vector1.cross(vector2);
    vector3.normalize();
}

Matrix44 inverseChangeOfCoordinateSystemMatrix(const Vector3& xAxis,    const Vector3& yAxis,    const Vector3& zAxis)
{

    Matrix44 m(xAxis.x(), xAxis.x(), xAxis.x(), 0,
               yAxis.y(), yAxis.y(), yAxis.y(), 0,
               zAxis.z(), zAxis.z(), zAxis.z(), 0,
               0,         0,         0,         1);

    return m;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector3 BRDF::CosineWeightedHemisphereBRDFDirectionSampling(const IntersectionInformation& intersection) const
{
#pragma region CosineCreationRandomVector

    //double u1 = sdisBRDF(sgenBRDF);
    //double u2 = sdisBRDF(sgenBRDF);

    ////Sample uniform disk.
    //double r = sqrt(u1);
    //double theta = 2.0 * math::gmPI * u2;
    //double x = r * cos(theta);
    //double z = r * sin(theta);

    ////Sample cosine weighted hemisphere sample.
    //double y = sqrt(std::fmax(0.0, 1.0 - u1));

    ////Setup obtained sample.
    //Vector3 vectorSampleFromHemisphere(x, y, z);

    ////Generate tangent space (local coordinates) using normal.
    //Vector3 tangent;
    //Vector3 bitangent;

    //generateCoodinateSystem(intersection.m_normal, tangent, bitangent);

    ////Convert sample in world coordinate, considering canonical base.
    //Matrix44 m = inverseChangeOfCoordinateSystemMatrix(bitangent, intersection.m_normal, tangent);
    //auto transformed = vectorSampleFromHemisphere * m;
    //Vector3 sampleInWorldCoordinate;
    //sampleInWorldCoordinate.setX(transformed.x());
    //sampleInWorldCoordinate.setY(transformed.y());
    //sampleInWorldCoordinate.setZ(transformed.z());

    //return sampleInWorldCoordinate;
#pragma endregion

    return CreateHemiSphereRandomVector(intersection.m_normal);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
LambertianBRDF::LambertianBRDF(const Vector4& color)
{
    m_color = color;
    m_weightedColor = color / math::gmPI;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector4 LambertianBRDF::sample(Vector3& wi, const Vector3& wo, const IntersectionInformation& intersectionInformation)
{
    wi = CosineWeightedHemisphereBRDFDirectionSampling(intersectionInformation);

    return m_weightedColor;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
double LambertianBRDF::pdf(const Vector3& wi, const Vector3 w0, const IntersectionInformation& intersectionInfo)
{
    return ((fabs(wi.dot(intersectionInfo.m_normal))) / math::gmPI);
}


