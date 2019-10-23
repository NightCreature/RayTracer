#include "WorkerThread.h"

#include "JobSystem.h"

///-----------------------------------------------------------------------------
///! @brief Main function of this thread
///! @remark
///-----------------------------------------------------------------------------
int WorkerThread::WorkerFunction()
{
    while(isAlive())
    {
        auto& queue = m_jobSystem->GetJobQueue();
        auto workLoad = queue.GetNextWorkLoad();
        if (workLoad.m_job != nullptr)
        {
            //Execute this workload

            workLoad.m_job->Execute(workLoad.m_arguments, m_index);
        }

        m_jobSystem->WorkerThreadSleeping(m_index);
        pauzeThread();
    }

    return 0;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void WorkerThread::pauzeThread()
{
    //Probably need to sleep on a condition variable here that the jobsystem can set
    WaitForSingleObject(m_workAvailableHandle, INFINITE);
}
