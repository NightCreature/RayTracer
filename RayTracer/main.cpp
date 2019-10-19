#include "Camera.h"
#include "RenderOptions.h"
#include "Scene.h"
#include "Utilities.h"

#include <filesystem>
#include <iostream>

int main()
{
    Sphere sphere;
    sphere.m_position = Vector3::zero();
    sphere.m_radius = 1.0;
    Scene scene;
    scene.m_shapes.push_back(&sphere);

    Camera cam;
    cam.positionCamera(Vector3(0.0, 0.0, -50.0), Vector3::zero(), Vector3::yAxis());
    
    RenderOptions renderOptions(cam, 512, 512, math::gmPI / 4.0);
    auto maxSampleCount = 10;

    std::vector<Vector4> imagePixels;
    imagePixels.resize(renderOptions.m_outputWidth * renderOptions.m_outputHeight);

    for (size_t x = 0; x < renderOptions.m_outputWidth; ++x)
    {
        for (size_t y = 0; y < renderOptions.m_outputHeight; ++y)
        {
            Vector4& colorAccumulator = imagePixels[y * renderOptions.m_outputWidth + x];
            for (size_t sampleCount = 0; sampleCount < maxSampleCount; ++sampleCount)
            {
                Ray ray;
                ray.CreateRay(renderOptions.m_cam.getEye(), renderOptions, x, y);

                //send ray into scene
                colorAccumulator += scene.TraceRay(ray);
            }

            colorAccumulator /= maxSampleCount;
        }
    }


    //Save output image
    auto path = std::filesystem::current_path();
    auto fileName = path / "output.bmp";
    auto savedImage = SaveImage(fileName.string(), imagePixels, renderOptions);

    return savedImage;
}