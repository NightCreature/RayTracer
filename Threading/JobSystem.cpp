#include "JobSystem.h"

#include <atomic>
#include <sstream>

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobSystem::JobSystem(size_t numThreads)
{
    m_workerThreads.resize(numThreads);

    m_eventHandle = CreateEvent(NULL, TRUE, FALSE, "SignalWorkAvailable");

    size_t index = 0;
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_thread.SetJobsystem(this, index, m_eventHandle);
        threadStatus.m_working = false;
        ++index;
    }

    //Set the work available even to unsignaled
    ResetEvent(m_eventHandle);

    //Start all threads
    index = 0;
    for (auto& threadStatus : m_workerThreads)
    {
        std::stringstream str;
        str << "WorkerThread" << index;
        threadStatus.m_working = true;
        threadStatus.m_thread.createThread(1024*1024, str.str());
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobSystem::~JobSystem()
{
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_thread.stopThread();
    }

    SetEvent(m_eventHandle);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WorkerThreadSleeping(size_t index)
{
    std::scoped_lock<std::mutex> sl(m_finishedMutex);
    m_workerThreads[index].m_working = false;

}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WorkerThreadActive(size_t index)
{
    std::scoped_lock<std::mutex> sl(m_finishedMutex);
    m_workerThreads[index].m_working = false;

}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::SignalWorkAvailable()
{
    SetEvent(m_eventHandle);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool JobSystem::IsFinished()
{
    std::scoped_lock<std::mutex> sl(m_finishedMutex);
    std::atomic<bool> retVal = true;
    if (m_jobQueue.m_jobs.size() > 0)
    {
        return false;
    }
    for (auto& threadStatus : m_workerThreads)
    {
        bool val = retVal;
        val &= !(threadStatus.m_working);
        retVal = val;
    }

    return retVal;
}
