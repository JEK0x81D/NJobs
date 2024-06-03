#include "NJobFiberPool.h"

#include "NJobFiber.h"


std::shared_ptr<NJobs::NJobFiber> NJobs::NJobFiberPool::GetFiber()

{
    if (m_fibers.empty())
    {
        return std::make_shared<NJobFiber>();
    }
    else
    {
        std::shared_ptr<NJobFiber> fiber = m_fibers.top();
        m_fibers.pop();
        return fiber;
    }
}

void NJobs::NJobFiberPool::ReturnFiber(std::shared_ptr<NJobFiber> fiber)
{
    m_fibers.push(fiber);
}
