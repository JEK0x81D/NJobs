#pragma once
#include <memory>
#include <stack>

namespace NJobs
{
    class NJobFiber;
    class NJobThread;

    class NJobFiberPool
    {
    public:
        NJobFiberPool() = default;

        std::shared_ptr<NJobFiber> GetFiber();
        void ReturnFiber(std::shared_ptr<NJobFiber> fiber);

    private:
        std::stack<std::shared_ptr<NJobFiber>> m_fibers;
    };
};