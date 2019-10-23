#pragma once

#include <vector>
#include <windows.h>

class Job
{
public:
    Job() = default;
    virtual ~Job() = default;

    virtual void Execute(void* arguments, size_t index) = 0;
};

struct Workload
{
    Job* m_job = nullptr;
    void* m_arguments = nullptr;
};


class SimplePrintTask : public Job
{
public:
    virtual void Execute(void* arguments, size_t index);
};

class JobQueue
{
public:
    JobQueue();

    void AddJob(Job* job, void* arguments);

    Workload GetNextWorkLoad();

    CRITICAL_SECTION m_criticalSection;
    std::vector<Workload> m_jobs;
};