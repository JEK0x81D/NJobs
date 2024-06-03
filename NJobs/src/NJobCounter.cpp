#include "NJobCounter.h"

#include "NJobFiber.h"
#include "NJobSystem.h"
#include "NJobThread.h"

void NJobs::NJobCounter::Increment()
{
    ++m_count;
}
void NJobs::NJobCounter::Decrement()
{
    --m_count;

    if (m_count == 0)
    {
        for (auto &fiberToResume : m_waitingFibers)
        {
            // Mark as being ready to resume and add to resumable list.
            fiberToResume->m_isWaiting = false;
            fiberToResume->GetJobThread()->GetJobSystem()->AddResumableFiber(fiberToResume);
        }

        m_waitingFibers.clear();
    }
}

void NJobs::NJobCounter::WaitForCounter(const NJobFiberHandle &currentFiber)
{
    if (m_count == 0)
    {
        return;
    }

    // Add the current fiber in the current context to waiting list.
    m_waitingFibers.emplace_back(currentFiber);
    currentFiber->m_isWaiting = true;

    // Release control to the main job fiber so we can do other stuff..
    currentFiber->GetJobThread()->YieldExecution();
}
