#pragma once

#include <memory>

#include "NJobCounter.h"
#include "NJobFiber.h"

namespace NJobs
{
    typedef void (*NJobFunction)(NJobFiberHandle fiberHandle, void* data);
    typedef void *NJobData;

    enum class EJobPriority : uint8_t
    {
        LOW,
        NORMAL,
        HIGH,
        CRITICAL
    };

    struct NJobComparator
    {
        bool operator()(std::shared_ptr<NJob> a, std::shared_ptr<NJob> b) const;
    };

    struct NJobDescriptor
    {
        NJobDescriptor(NJobFunction function, std::shared_ptr<NJobCounter> counter, EJobPriority priority)
            : m_function(function), m_counter(counter), m_priority(priority)
        {
        }

        NJobFunction m_function;
        std::shared_ptr<NJobCounter> m_counter;
        EJobPriority m_priority;
    };

    class NJob
    {
    public:
        NJob(NJobDescriptor descriptor, NJobData data);
        NJob(NJob &Other) = delete;
        NJob(NJob &&Other) = delete;
        NJob() = delete;

        void Execute();
        void Finish();

        void SetCurrentFiber(NJobFiberHandle currentFiber) { m_currentFiber = currentFiber; }

        bool IsAlive() const { return m_alive; }

        EJobPriority GetPriority() const { return m_descriptor.m_priority; }

    private:
        NJobDescriptor m_descriptor;
        NJobData m_data{};
        NJobFiberHandle m_currentFiber;
        std::atomic_bool m_alive{true};
    };
}
