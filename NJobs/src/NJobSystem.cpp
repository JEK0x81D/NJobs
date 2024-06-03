#include "NJobSystem.h"

#include "NJobQueue.h"
#include "NJobThread.h"

void NJobs::NJobSystem::Initialize()
{
    m_numThreads = std::thread::hardware_concurrency();

    if (m_numThreads <= 0)
    {
        m_numThreads = 1;
    }

    m_jobQueue = std::make_shared<NJobQueue>();
    m_fiberPool = std::make_shared<NJobFiberPool>();
    m_threads = std::vector<std::thread>(m_numThreads);

    // Initialize job threads.
    for (uint32_t i = 0; i < m_numThreads; ++i)
    {
        std::shared_ptr<NJobThread> jobThread = std::make_shared<NJobThread>();
        m_jobSystems.push_back(jobThread);
    }

}

void NJobs::NJobSystem::StartThreads()
{
    // Setup threads and run kick off job runner.
    for (uint32_t i = 0; i < m_numThreads; ++i)
    {
        SetThreadAffinityMask(m_threads[i].native_handle(), DWORD_PTR(1) << i);
        m_threads[i] = std::thread(&RunJobSystem, m_jobSystems[i], shared_from_this());
    }
}

void NJobs::NJobSystem::JoinThreads()
{
    for (auto &thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void NJobs::NJobSystem::AddJobToQueue(const std::shared_ptr<NJob> &job) const
{
    m_jobQueue->AddJobToQueue(job);
}

void NJobs::NJobSystem::AddResumableFiber(const NJobFiberHandle &fiberHandle)
{
    std::lock_guard<std::mutex> lock(m_waitingFibersMutex);

    m_waitingFibers.push_back(fiberHandle);
}

NJobs::NJobFiberHandle NJobs::NJobSystem::GetRunnableFiber()
{
    {
        std::lock_guard<std::mutex> lock(m_waitingFibersMutex);

        if (!m_waitingFibers.empty())
        {
            NJobFiberHandle fiber = m_waitingFibers.back();
            m_waitingFibers.pop_back();

            return fiber;
        }
    }

    std::shared_ptr<NJob> job = m_jobQueue->GetNextJob();

    if (job)
    {
        std::shared_ptr<NJobFiber> fiber = m_fiberPool->GetFiber();
        NJobFiberHandle fiberHandle(fiber, m_fiberPool);
        job->SetCurrentFiber(fiberHandle);
        fiberHandle->AssignJob(job);

        return fiberHandle;
    }

    return NJobFiberHandle();
}

void NJobs::NJobSystem::RunJobSystem(const std::shared_ptr<NJobThread> &jobThread,
                                     const std::shared_ptr<NJobSystem> &jobSystem)
{
    jobThread->Initialize(jobSystem);
    jobThread->ExecuteJobs();
}

