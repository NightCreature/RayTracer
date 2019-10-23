#include "Camera.h"
#include "RenderOptions.h"
#include "Scene.h"
#include "Utilities.h"

#include <filesystem>
#include <iostream>

#include "Threading/Job.h"
#include "Threading/JobSystem.h"

int main()
{
    Scene scene;
    scene.DeserialiseScene("scene.xml");

    Camera cam;
    cam.positionCamera(Vector3(0.0, 0.0, -12.0), Vector3::zero(), Vector3::yAxis());
    
    RenderOptions renderOptions(cam, math::gmPI / 4.0);
    renderOptions.Deserialise("rendersettings.xml");
    renderOptions.m_usePathTracing = true;

    std::vector<Vector4> imagePixels;
    imagePixels.resize(renderOptions.m_outputWidth * renderOptions.m_outputHeight);

    SimplePrintTask tasks[10];
    JobSystem jobSystem(3);
    auto& jobQueue = jobSystem.GetJobQueue();
    for (size_t counter = 0; counter < 10; ++counter)
    {
        jobQueue.AddJob(&tasks[0], nullptr);
    }
    jobSystem.SignalWorkAvailable();

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
                colorAccumulator += scene.TraceRay(ray, renderOptions.m_numberOfBounces);
            }

            colorAccumulator /= static_cast<double>(renderOptions.m_numberOfSamples);
        }
    }


    //Save output image
    auto path = std::filesystem::current_path();
    auto fileName = path / "output.bmp";
    //auto savedImage = SaveImage(fileName.string(), imagePixels, renderOptions);

    //return savedImage;
    return 0;
}