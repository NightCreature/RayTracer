#include "Camera.h"
#include "RenderOptions.h"
#include "Scene.h"
#include "Utilities.h"

#include <filesystem>
#include <iostream>

int main()
{
    Sphere sphere;
    sphere.m_position = Vector3(-6, 0, 0);
    sphere.m_radius = 6;
    sphere.m_material.m_diffuseColor = Vector4(1, 0, 0, 0);
    Scene scene;
    scene.m_shapes.push_back(&sphere);

    Sphere sphere2;
    sphere2.m_position = Vector3(6, 0, 0);
    sphere2.m_radius = 6;
    sphere2.m_material.m_diffuseColor = Vector4(0, 0, 1, 0);
    scene.m_shapes.push_back(&sphere2);


    Sphere sphere3;
    sphere3.m_position = Vector3(0, 6, 0);
    sphere3.m_radius = 6;
    sphere3.m_material.m_diffuseColor = Vector4(0, 1, 0, 0);
    scene.m_shapes.push_back(&sphere3);

    Camera cam;
    cam.positionCamera(Vector3(0.0, 0.0, 12.0), Vector3::zero(), Vector3::yAxis());
    
    RenderOptions renderOptions(cam, math::gmPI / 4.0);
    renderOptions.Deserialise("rendersettings.xml");
    renderOptions.m_usePathTracing = true;

    std::vector<Vector4> imagePixels;
    imagePixels.resize(renderOptions.m_outputWidth * renderOptions.m_outputHeight);

    for (size_t x = 0; x < renderOptions.m_outputWidth; ++x)
    {
        for (size_t y = 0; y < renderOptions.m_outputHeight; ++y)
        {
            Vector4& colorAccumulator = imagePixels[y * renderOptions.m_outputWidth + x];
            for (size_t sampleCount = 0; sampleCount < renderOptions.m_numberOfSamples; ++sampleCount)
            {
                Ray ray;
                ray.CreateRay(renderOptions, x, y);

                //send ray into scene
                colorAccumulator += scene.TraceRay(ray);
            }

            colorAccumulator /= static_cast<double>(renderOptions.m_numberOfSamples);
        }
    }


    //Save output image
    auto path = std::filesystem::current_path();
    auto fileName = path / "output.bmp";
    auto savedImage = SaveImage(fileName.string(), imagePixels, renderOptions);

    return savedImage;
}