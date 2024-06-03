#include "NJob.h"


namespace NJobs {
    class NJobFiber;
}

bool NJobs::NJobComparator::operator()(std::shared_ptr<NJob> a, std::shared_ptr<NJob> b) const
{
    return a->GetPriority() < b->GetPriority();
}

NJobs::NJob::NJob(NJobDescriptor descriptor, NJobData data)
    : m_descriptor(std::move(descriptor)), m_data(data)
{
    if (m_descriptor.m_counter)
    {
        m_descriptor.m_counter->Increment();
    }
}

void NJobs::NJob::Execute()
{
    if (!m_alive)
    {
        return;
    }

    m_descriptor.m_function(m_currentFiber, m_data);
    Finish();
}

void NJobs::NJob::Finish()
{
    if (!m_alive)
    {
        return;
    }

    m_alive = false;

    if (m_descriptor.m_counter)
    {
        m_descriptor.m_counter->Decrement();
    }
}
