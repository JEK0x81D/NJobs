#pragma once

#include <mutex>
#include <queue>

#include "NJob.h"

namespace NJobs
{
    class NJobQueue
    {
    public:
        void AddJobToQueue(const std::shared_ptr<NJob> &job);
        std::shared_ptr<NJob> GetNextJob();

    private:
        std::priority_queue<std::shared_ptr<NJob>, std::vector<std::shared_ptr<NJob>>, NJobComparator> m_pendingJobs;
        std::mutex m_mutex;
    };
}
