#pragma once
#include <atomic>
#include <mutex>
#include <vector>

namespace NJobs
{
    class NJobFiberHandle;
    class NJobFiber;
    class NJob;

    class NJobCounter
    {
    public:
        void Increment();
        void Decrement();
        void WaitForCounter(const NJobFiberHandle &currentFiber);

    private:
        std::atomic_int m_count;
        std::vector<NJobFiberHandle> m_waitingFibers;
        std::mutex m_mutex;
    };
}