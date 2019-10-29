#include "PixelBlockJob.h"

#include "RenderOptions.h"
#include "Scene.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void PixelBlockJob::Execute(size_t threadIndex)
{
    m_timer.update();
    size_t now = m_timer.getTimeStamp();
    std::stringstream str("");
    str << "<<<<<" << threadIndex << ">>>>>\n";
    str << "Starting PixelJob on Thread " << threadIndex << "\n";
    str << "With Settings:\n" << "Start Row: " << m_params.m_startIndex << "\nEnd Row: " << m_params.m_endIndex <<"\n";
    str << "<<<<<" << threadIndex << ">>>>>\n";
    OutputDebugString(str.str().c_str());
    size_t width = m_params.m_renderOptions->m_outputWidth;
    size_t noSamples = m_params.m_renderOptions->m_numberOfSamples;
    std::vector<Vector4> threadLocalArray;
    size_t numberOfRowsToGenerate = m_params.m_endIndex - m_params.m_startIndex;
    //threadLocalArray.resize(width * numberOfRowsToGenerate);
    auto& pixelArrayRef = *(m_params.m_pixelArray);

    //std::vector<size_t> pixelTime;
    //pixelTime.resize(width * numberOfRowsToGenerate);

    for (size_t y = 0; y < numberOfRowsToGenerate; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            Vector4 colorAccumulator;
            //pixelTime[y * width + x] = m_timer.getTimeStamp();
            for (size_t sampleCount = 0; sampleCount < noSamples; ++sampleCount)
            {
                Ray ray;
                ray.CreateRay(*(m_params.m_renderOptions), x, y + m_params.m_startIndex);

                //send ray into scene
                colorAccumulator += m_params.m_scene->TraceRay(ray, m_params.m_renderOptions->m_numberOfBounces);
            }

            colorAccumulator /= static_cast<double>(noSamples);
            pixelArrayRef[(y + m_params.m_startIndex) * width + x] = colorAccumulator;
            //str.str("");
            //str << "Finished Row: " << y << "on thread: " << threadIndex << "\n";
            //OutputDebugString(str.str().c_str());
            //pixelTime[y * width + x] = m_timer.getTimeStamp() - pixelTime[y * width + x];
        }
    }

    
    //memcpy(&(pixelArrayRef[m_params.m_startIndex]), &(threadLocalArray[0]), sizeof(Vector4) * numberOfRowsToGenerate * width);

    size_t end = m_timer.getTimeStamp();
    str.str("");
    str << "<<<<<" << threadIndex << ">>>>>\n";
    //for (size_t interval : pixelTime)
    //{
    //    str << "Single ray took: " << interval / m_timer.getResolution() << "s"  << "\n";
    //}
    str << "Finished PixelJob on Thread " << threadIndex << " in: " << (end - now) / m_timer.getResolution() << "s\n";
    str << "<<<<<" << threadIndex << ">>>>>\n";
    OutputDebugString(str.str().c_str());
}
