#pragma once

#include "Job.h"
#include "WorkerThread.h"

#include <vector>

class JobSystem
{
public:
    JobSystem(size_t numThreads);

    const JobQueue& GetJobQueue() const { return m_jobQueue; }
    JobQueue& GetJobQueue() { return m_jobQueue; }

    void WorkerThreadSleeping(size_t index);

    void SignalWorkAvailable();

    struct ThreadStatus
    {
        WorkerThread m_thread;
        bool m_working;
    };

    std::vector<ThreadStatus> m_workerThreads;
    JobQueue m_jobQueue;

    HANDLE m_eventHandle;
};

