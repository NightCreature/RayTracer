#include "Utilities.h"

#include "Math/MathUtilityFunctions.h"
#include "Math/matrix33.h"
#include "Math/vector3.h"
#include "Ray.h"

#include <random>
#include <vector>
#include <windows.h>

#undef max

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool SaveImage(const std::string& fileName, std::vector<Vector4>& pixels, const RenderOptions& renderOptions)
{
    // allocate memory for our bitmap BGR U8 image
    std::vector<Color> outPixels;
    outPixels.resize(pixels.size());

    // convert from RGB F32 to BGR U8
    for (size_t counter = 0; counter < pixels.size(); ++counter)
    {
        Vector4& src = pixels[counter];
        Color& dest = outPixels[counter];

        // apply gamma correction
        Vector4 correctedPixel(src);// pow(src.x(), 1.0 / 2.2), pow(src.y(), 1.0 / 2.2), pow(src.z(), 1.0 / 2.2), 1.0);

        // clamp and convert
        dest.r = uint8_t(math::clamp(correctedPixel.x() * 255.0, 0.0, 255.0));
        dest.g = uint8_t(math::clamp(correctedPixel.y() * 255.0, 0.0, 255.0));
        dest.b = uint8_t(math::clamp(correctedPixel.z() * 255.0, 0.0, 255.0));
    }

    // write the bitmap

    // open the file if we can
    FILE* file;
    fopen_s(&file, fileName.c_str(), "wb");
    if (!file)
        return false;

    // make the header info
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER infoHeader;

    header.bfType = 0x4D42;
    header.bfReserved1 = 0;
    header.bfReserved2 = 0;
    header.bfOffBits = 54;

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = (LONG)(renderOptions.m_outputWidth);
    infoHeader.biHeight = (LONG)(renderOptions.m_outputHeight);
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biCompression = 0;
    infoHeader.biSizeImage = (DWORD)(outPixels.size() * 3);
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biClrImportant = 0;

    header.bfSize = infoHeader.biSizeImage + header.bfOffBits;

    // write the data and close the file
    fwrite(&header, sizeof(header), 1, file);
    fwrite(&infoHeader, sizeof(infoHeader), 1, file);
    //std::reverse(outPixels.begin(), outPixels.end());
    fwrite(&outPixels[0], infoHeader.biSizeImage, 1, file);
    fclose(file);
    return true;
}

std::random_device srd;
std::mt19937 sgen(srd());
std::uniform_real_distribution<> sdis(-1.0, 1.0);

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector3 CreateRandomUnitVector()
{
    Vector3 dir = Vector3(sdis(sgen), sdis(sgen), sdis(sgen));
    while (dir.dot(dir) >= 1)
    {
        dir = Vector3(sdis(sgen), sdis(sgen), sdis(sgen));
    }

    return dir;
}

Matrix33 GetTangentSpace(Vector3 normal)
{
    // Choose a helper vector for the cross product
    Vector3 helper = Vector3(1, 0, 0);
    if (abs(normal.x()) > 0.99)
        helper = Vector3(0, 0, 1);
    // Generate vectors
    Vector3 tangent = cross(normal, helper);
    tangent.normalize();
    Vector3 binormal = cross(normal, tangent);
    binormal.normalize();
    return Matrix33(tangent, binormal, normal);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector3 CreateHemiSphereRandomVector(const Vector3& normal)
{
    // Uniformly sample hemisphere direction
    double cosTheta = sdis(sgen);
    double sinTheta = sqrt(std::max(0.0, 1.0 - cosTheta * cosTheta));
    double phi = 2 * math::gmPI * sdis(sgen);
    Vector3 tangentSpaceDir = Vector3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
    // Transform direction to world space
    auto tangentSpace = GetTangentSpace(normal);
    return multiply(tangentSpaceDir, tangentSpace);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector3 Reflect(const Ray& ray, const Vector3& normal)
{
    return ray.m_direction - 2.f * ray.m_direction.dot(normal) * normal;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Vector3 Refract(const Ray& ray, const Vector3& normal, double refractionIndex )
{
    Vector3 dir;
    double cosi = math::clamp(ray.m_direction.dot(normal), -1.0, 1.0);
    double etai = 1;
    double etat = refractionIndex;
    Vector3 Normal = normal;
    if (cosi < 0)
    {
        cosi = -cosi;
    }
    else
    {
        std::swap(etai, etat);
        Normal = -normal;
    }

    double eta = etai / etat;
    double k = 1 - eta * eta * (1 - cosi * cosi);
    if (k > 0)
    {
        dir = eta * ray.m_direction + (eta * cosi - sqrt(k)) * Normal;
    }                        
    
    return dir;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Fresnel(const Ray& ray, const Vector3& normal, double refractionIndex, double& fresnelFactor)
{
    double cosi = math::clamp(ray.m_direction.dot(normal), -1.0, 1.0);
    double etai = 1;
    double etat = refractionIndex;
    if (cosi > 0) 
    {
        std::swap(etai, etat); 
    }
    // Compute sini using Snell's law
    double sint = etai / etat * sqrt(std::max(0., 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        fresnelFactor = 1;
    }
    else {
        double cost = sqrt(std::max(0., 1 - sint * sint));
        cosi = fabs(cosi);
        double Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        double Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        fresnelFactor = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, transmittance is given by:
    // kt = 1 - kr;
}
