#pragma once
#include <memory>
#include <windows.h>


namespace NJobs {
    class NJobCounter;
}

namespace NJobs
{
    class NJobFiberPool;
    class NJob;
    class NJobThread;

    class NJobFiber
    {
    public:
        NJobFiber();
        ~NJobFiber();

        void SwitchToFiber() const;
        void AssignJobThread(const std::shared_ptr<NJobThread> &jobThread);
        void AssignJob(const std::shared_ptr<NJob> &job);
        std::shared_ptr<NJobThread> GetJobThread() const { return m_jobThread; }

        bool HasFinished() const;
        bool IsWaiting() const { return m_isWaiting; }

        static void ConvertThreadToFiber()
        {
            if (!IsThreadAFiber())
            {
                ::ConvertThreadToFiber(nullptr);
            }
        }

    private:
        LPVOID m_fiber{nullptr};
        std::shared_ptr<NJob> m_job{nullptr};
        std::shared_ptr<NJobThread> m_jobThread{nullptr};
        bool m_isWaiting{false};

        static void WINAPI FiberFunction(void *parameter);

        friend NJobCounter;
    };

    // Handle for fibers. When fiber is complete (job is finished), it will return to pool.
    // In the case that a fiber is incomplete (job has yielded), it will not be destroyed, and should be resumed later
    // when the counter that stops it is decremented.
    // TODO: Move the return to pool into NJob?
    class NJobFiberHandle
    {
    public:
        NJobFiberHandle(std::shared_ptr<NJobFiber> fiber, std::shared_ptr<NJobFiberPool> pool)
            : m_fiber(fiber), m_pool(pool) {}
        NJobFiberHandle() = default;
        NJobFiberHandle(const NJobFiberHandle& other)
            : m_fiber(other.m_fiber), m_pool(other.m_pool) {}
        NJobFiberHandle(NJobFiberHandle&& other) noexcept
             : m_fiber(std::move(other.m_fiber)), m_pool(std::move(other.m_pool)) {}
        ~NJobFiberHandle();

        NJobFiberHandle& operator=(const NJobFiberHandle& other)
        {
            if (this != &other)
            {
                m_fiber = other.m_fiber;
                m_pool = other.m_pool;
            }

            return *this;
        }

        NJobFiberHandle& operator=(NJobFiberHandle&& other) noexcept
        {
            if (this != &other)
            {
                m_fiber = std::move(other.m_fiber);
                m_pool = std::move(other.m_pool);
            }

            return *this;
        }

        bool IsValid() const { return m_fiber != nullptr; }

        std::shared_ptr<NJobFiber> operator->() const { return m_fiber; }

    private:
        std::shared_ptr<NJobFiber> m_fiber{nullptr};
        std::shared_ptr<NJobFiberPool> m_pool{nullptr};
    };

}
