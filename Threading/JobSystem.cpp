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
    
    std::atomic_int enforceOrder;

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
        std::stringstream str("WorkerThread");
        str << index;
        threadStatus.m_working = true;
        threadStatus.m_thread.createThread(1024*1024, str.str());
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WorkerThreadSleeping(size_t index)
{
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
