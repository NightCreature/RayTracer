#include "Utilities.h"

#include <random>
#include <vector>
#include <windows.h>

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
