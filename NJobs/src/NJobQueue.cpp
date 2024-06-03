#include "NJobQueue.h"

void NJobs::NJobQueue::AddJobToQueue(const std::shared_ptr<NJob> &job)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_pendingJobs.push(job);
}

std::shared_ptr<NJobs::NJob> NJobs::NJobQueue::GetNextJob()
{
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_pendingJobs.empty())
    {
        return nullptr;
    }

    std::shared_ptr<NJob> job = m_pendingJobs.top();
    m_pendingJobs.pop();

    return job;
}
