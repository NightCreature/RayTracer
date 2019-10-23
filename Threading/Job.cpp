#include "Job.h"

#include <sstream>

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobQueue::JobQueue()
{
    InitializeCriticalSectionAndSpinCount(&m_criticalSection, 50);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobQueue::AddJob(Job* job, void* arguments)
{
    EnterCriticalSection(&m_criticalSection);
    Workload load;
    load.m_job = job;
    load.m_arguments = arguments;
    m_jobs.push_back(load);
    LeaveCriticalSection(&m_criticalSection);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Workload JobQueue::GetNextWorkLoad()
{
    Workload workload;
    //Use critical sections for now
    EnterCriticalSection(&m_criticalSection);
    if (!m_jobs.empty())
    {
        workload = *m_jobs.rbegin();
        m_jobs.pop_back();
    }
    LeaveCriticalSection(&m_criticalSection);
    return workload;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void SimplePrintTask::Execute(void* arguments, size_t index)
{
    std::stringstream str("");
    str << "Thread " << index << " executing\n";
    OutputDebugString(str.str().c_str());
}
