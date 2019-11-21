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
#include <chrono>
#include <filesystem>
#include <iostream>
#include <sstream>

std::atomic_size_t intersectionCount = 0;

int main()
{
    OutputDebugString("Read Scene and RenderSettings\n");
    Scene scene;
    scene.DeserialiseScene("scene.xml");

    std::stringstream objectCount("");
    objectCount << "Number of objects in the scene: " << scene.m_shapes.size() << "\n";
    OutputDebugString(objectCount.str().c_str());

    Camera cam;
    cam.positionCamera(Vector3(0.0, 0.0, 12), Vector3::zero(), Vector3::yAxis());
    
    RenderOptions renderOptions(cam, math::gmPI / 2.0);
    renderOptions.Deserialise("rendersettings.xml");
    renderOptions.m_usePathTracing = true;

    std::vector<Vector4> imagePixels;
    imagePixels.resize(renderOptions.m_outputWidth * renderOptions.m_outputHeight);

    JobSystem jobSystem(renderOptions.m_numberOfWorkerThreads);
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

    size_t numberOfJobs = renderOptions.m_numberOfTasks;
    if (renderOptions.m_outputHeight / renderOptions.m_numberOfTasks != 0)
    {
        size_t pixelRowsPerJob = (renderOptions.m_outputHeight / renderOptions.m_numberOfTasks);
        size_t pixelRowsLeft = renderOptions.m_outputHeight - pixelRowsPerJob * numberOfJobs;
        size_t additionalJobs = static_cast<size_t>(std::ceil(pixelRowsLeft / static_cast<double>(pixelRowsPerJob)));
        numberOfJobs += additionalJobs;
    }
    std::vector<PixelBlockJob> pixelJobs;
    pixelJobs.resize(numberOfJobs);
    for (size_t counter = 0; counter < numberOfJobs; ++counter)
    {
        param.m_startIndex = param.m_endIndex;
        param.m_endIndex = param.m_startIndex + renderOptions.m_outputHeight / renderOptions.m_numberOfTasks;
        if (param.m_endIndex > renderOptions.m_outputHeight)
        {
            param.m_endIndex = renderOptions.m_outputHeight;
        }
        pixelJobs[counter] = PixelBlockJob(param);
    }

    for (auto& job : pixelJobs)
    {
        jobQueue.AddJob(&job);
    }

    OutputDebugString("Start Work on Jobs\n");
    //jobSystem.SignalWorkAvailable();

    PerformanceTimer timer;
    timer.update();
    size_t timeStamp = timer.getTimeStamp();
    std::chrono::high_resolution_clock performanceClock;
    auto start = performanceClock.now();

    jobSystem.WaitfForJobsToFinish();

    auto timerDuration = (timer.getTimeStamp() - timeStamp) / timer.getResolution();
    auto end = performanceClock.now();
    std::chrono::duration<double> duration = end - start;

    std::stringstream str("");
    auto timeSpentDouble = 0.0;
    auto lessThenSeconds = modf(timerDuration, &timeSpentDouble);
    lessThenSeconds *= 10000000000;
    auto timeSpent = static_cast<size_t>(timeSpentDouble);
    auto seconds = timeSpent % 60;
    timeSpent /= 60;
    auto minutes = timeSpent % 60;
    timeSpent /= 60;
    auto hours = timeSpent % 24;
    auto days = timeSpent / 24;
    str << "Jobs done time elapsed on main: " <<days << " days " << hours << ":" << minutes << ":" << seconds << "." << static_cast<size_t>(lessThenSeconds) << "\n";
    str << "Chrono elpased time: " << std::chrono::duration_cast<std::chrono::seconds>(duration).count() << "\n";
    str << "Chrono elpased time: " << duration.count() << "\n";
    str << "Number of Intersections: " << intersectionCount << "\n";
    str << "<<<<MAIN>>>>>\n";
    OutputDebugString(str.str().c_str());

    OutputDebugString("Start Saving Image\n");
    //Save output image
    auto path = std::filesystem::current_path();

    std::stringstream converter;
    converter << "Samples " << renderOptions.m_numberOfSamples << " Bounces " << renderOptions.m_numberOfBounces << " Objects " << scene.m_shapes.size() << " " << renderOptions.m_outputWidth << "x" << renderOptions.m_outputHeight << "" << renderOptions.m_outputFileName;
    auto fileName = path / "Images" / converter.str();
    OutputDebugString("Saving To: ");
    OutputDebugString(fileName.string().c_str());
    OutputDebugString("\n");
    auto savedImage = SaveImage(fileName.string(), imagePixels, renderOptions);

    OutputDebugString("Finished Saving Image\n");
    return savedImage;
}