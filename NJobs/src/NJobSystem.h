#pragma once
#include <mutex>
#include <thread>
#include <vector>

namespace NJobs
{
    class NJobFiberPool;
    class NJobFiberHandle;
    class NJobQueue;
    class NJobThread;
    class NJob;

    // TODO: Update system so that main thread is converted instead of having to spawn job for main thread.
    class NJobSystem : public std::enable_shared_from_this<NJobSystem>
    {
    public:
        NJobSystem() = default;

        void Initialize();
        void StartThreads();
        void JoinThreads();

        void AddJobToQueue(const std::shared_ptr<NJob> &job) const;
        void AddResumableFiber(const NJobFiberHandle &fiberHandle);

        NJobFiberHandle GetRunnableFiber();

    private:
        std::vector<std::thread> m_threads;
        std::vector<std::shared_ptr<NJobThread>> m_jobSystems;
        std::shared_ptr<NJobQueue> m_jobQueue;
        std::shared_ptr<NJobFiberPool> m_fiberPool;

        // Fibers that are waiting to be resumed.
        std::vector<NJobFiberHandle> m_waitingFibers;
        std::mutex m_waitingFibersMutex;

        uint32_t m_numThreads;

        static void RunJobSystem(const std::shared_ptr<NJobThread> &jobThread, const std::shared_ptr<NJobSystem> &jobSystem);
    };
}
