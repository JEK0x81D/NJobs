#pragma once
#include <vector>

#include "NJobFiber.h"
#include "NJobFiberPool.h"

namespace NJobs
{
    class NJobSystem;
    class NJobCounter;
    class NJobQueue;
    class NJobFiberPool;

    class NJobThread : public std::enable_shared_from_this<NJobThread>
    {
    public:
        void Initialize(const std::shared_ptr<NJobSystem> &jobSystem)
        {
            m_mainFiber = ConvertThreadToFiber(nullptr);
            m_jobSystem = jobSystem;
        }

        void ExecuteJobs();
        void YieldExecution() const;

        std::shared_ptr<NJobSystem> GetJobSystem() const { return m_jobSystem; }

    private:
        LPVOID m_mainFiber{nullptr};
        std::shared_ptr<NJobSystem> m_jobSystem{nullptr};

        friend NJobCounter;
    };
}
