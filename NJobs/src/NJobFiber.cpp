#include "NJobFiber.h"
#include "NJob.h"
#include "NJobThread.h"

NJobs::NJobFiber::NJobFiber()
{
    m_fiber = ::CreateFiber(0, FiberFunction, this);
}

NJobs::NJobFiber::~NJobFiber()
{
    if (m_fiber)
    {
        ::DeleteFiber(m_fiber);
    }
}

void NJobs::NJobFiber::SwitchToFiber() const
{
    ::SwitchToFiber(m_fiber);
}

void NJobs::NJobFiber::AssignJobThread(const std::shared_ptr<NJobThread> &jobThread)
{
    m_jobThread = jobThread;
}

void NJobs::NJobFiber::AssignJob(const std::shared_ptr<NJob> &job)
{
    m_job = job;
}

bool NJobs::NJobFiber::HasFinished() const
{
    return !(m_job && m_job->IsAlive());
}

void NJobs::NJobFiber::FiberFunction(void *parameter)
{
    NJobFiber *fiber = static_cast<NJobFiber*>(parameter);

    while (true)
    {
        if (fiber->m_job)
        {
            fiber->m_job->Execute();
            fiber->m_job->Finish();
            fiber->m_job = nullptr;
        }

        fiber->m_jobThread->YieldExecution();
    }
}

NJobs::NJobFiberHandle::~NJobFiberHandle()
{
    if (IsValid() && m_fiber->HasFinished())
    {
        m_pool->ReturnFiber(m_fiber);
    }
}
