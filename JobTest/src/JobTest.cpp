
#include <iostream>
#include <memory>
#include <memory>
#include <memory>

#include "../../NJobs/src/NJob.h"
#include "../../NJobs/src/NJobSystem.h"
#include "../../NJobs/src/NJobThread.h"


void TestFunction(NJobs::NJobFiberHandle currentFiber, void *data)
{
    // for (int i = 0; i < 100; ++i)
    // {
    //     NOP_FUNCTION;
    // }
    std::cout << reinterpret_cast<int>(data) << '\n';
}

void MainJob(NJobs::NJobFiberHandle currentFiber, void *data)
{

    std::shared_ptr<NJobs::NJobSystem> jobSystem = currentFiber->GetJobThread()->GetJobSystem();

    while (true)
    {
        std::shared_ptr<NJobs::NJobCounter> jobCounter = std::make_shared<NJobs::NJobCounter>();
        NJobs::NJobDescriptor jobDescriptor(&TestFunction, jobCounter, NJobs::EJobPriority::NORMAL);

        for (int i = 0; i < 100; ++i)
        {
            std::shared_ptr<NJobs::NJob> job = std::make_shared<NJobs::NJob>(jobDescriptor, reinterpret_cast<void*>(i));
            jobSystem->AddJobToQueue(job);
        }

        jobCounter->WaitForCounter(currentFiber);
        std::cout << "MainJob finished." << '\n';
        // Do nothing.
    }
}

int main(int argc, char* argv[])
{
    std::shared_ptr<NJobs::NJobSystem> jobSystem = std::make_shared<NJobs::NJobSystem>();
    jobSystem->Initialize();
    jobSystem->StartThreads();

    NJobs::NJobDescriptor mainJobDescriptor(&MainJob, nullptr, NJobs::EJobPriority::NORMAL);
    std::shared_ptr<NJobs::NJob> mainJob = std::make_shared<NJobs::NJob>(mainJobDescriptor, nullptr);
    jobSystem->AddJobToQueue(mainJob);

    std::this_thread::sleep_for(std::chrono::seconds(500));

    jobSystem->JoinThreads();

    return 0;
}
