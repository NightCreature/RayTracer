#include "Camera.h"
#include "PixelBlockJob.h"
#include "RenderOptions.h"
#include "Scene.h"
#include "Threading/Job.h"
#include "Threading/JobSystem.h"
#include "Threading/Timer.h"
#include "Utilities.h"

#include <array>
#include <atomic>
#include <filesystem>
#include <iostream>
#include <sstream>

int main()
{
    OutputDebugString("Read Scene and RenderSettings\n");
    Scene scene;
    scene.DeserialiseScene("scene.xml");

    Camera cam;
    cam.positionCamera(Vector3(0.0, 0.0, -12.0), Vector3::zero(), Vector3::yAxis());
    
    RenderOptions renderOptions(cam, math::gmPI / 4.0);
    renderOptions.Deserialise("rendersettings.xml");
    renderOptions.m_usePathTracing = true;

    std::vector<Vector4> imagePixels;
    imagePixels.resize(renderOptions.m_outputWidth * renderOptions.m_outputHeight);

    constexpr size_t numberOfThreads = 3;

    JobSystem jobSystem(numberOfThreads);
    auto& jobQueue = jobSystem.GetJobQueue();


    //for (size_t x = 0; x < renderOptions.m_outputWidth; ++x)
    //{
    //    for (size_t y = 0; y < renderOptions.m_outputHeight; ++y)
    //    {
    //        Vector4& colorAccumulator = imagePixels[y * renderOptions.m_outputWidth + x];
    //        for (size_t sampleCount = 0; sampleCount < renderOptions.m_numberOfSamples; ++sampleCount)
    //        {
    //            Ray ray;
    //            ray.CreateRay(renderOptions, x, y);

    //            //send ray into scene
    //            colorAccumulator += scene.TraceRay(ray, renderOptions.m_numberOfBounces);
    //        }

    //        colorAccumulator /= static_cast<double>(renderOptions.m_numberOfSamples);
    //    }
    //}

    OutputDebugString("Prepare Jobs\n");

    PixelBlockJobParameters param;
    param.m_pixelArray = &imagePixels;
    param.m_renderOptions = &renderOptions;
    param.m_scene = &scene;


    std::array<PixelBlockJob, numberOfThreads> pixelJobs;
    for (auto& pixelJob : pixelJobs)
    {
        param.m_startIndex = param.m_endIndex;
        param.m_endIndex = param.m_startIndex + renderOptions.m_outputHeight / numberOfThreads;

        pixelJob = PixelBlockJob(param);

        jobQueue.AddJob(&pixelJob);
    }

    OutputDebugString("Start Work on Jobs\n");
    jobSystem.SignalWorkAvailable();

    PerformanceTimer timer;
    timer.update();
    size_t timeStamp = timer.getTimeStamp();

    OutputDebugString("Sleep main for 20 seconds\n");
    Sleep(20000);

    std::stringstream str("<<<<MAIN>>>>>\n");
    size_t timeStamp2 = timer.getTimeStamp();
    str << "Time Elapsed on main: " << (timeStamp2 - timeStamp) / timer.getResolution() << "s\n";
    str << "<<<<MAIN>>>>>\n";

    while (!jobSystem.IsFinished())
    {
        Sleep(5000);
    }


    str << "Jobs done time elapsed on main: " << (timer.getTimeStamp() - timeStamp2) / timer.getResolution() << "s\n";
    str << "<<<<MAIN>>>>>\n";
    OutputDebugString(str.str().c_str());

    OutputDebugString("Start Saving Image\n");
    //Save output image
    auto path = std::filesystem::current_path();
    auto fileName = path / "output.bmp";
    auto savedImage = SaveImage(fileName.string(), imagePixels, renderOptions);

    OutputDebugString("Finished Saving Image\n");
    return savedImage;
}