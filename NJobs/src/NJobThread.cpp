#include "NJobThread.h"

#include "NJobQueue.h"
#include "NJobSystem.h"

void NJobs::NJobThread::ExecuteJobs()
{
    while (true)
    {
        NJobFiberHandle fiberHandle = m_jobSystem->GetRunnableFiber();

        if (fiberHandle.IsValid())
        {
            fiberHandle->AssignJobThread(shared_from_this());
            fiberHandle->SwitchToFiber();
        }
    }
}
void NJobs::NJobThread::YieldExecution() const
{
    ::SwitchToFiber(m_mainFiber);
}
