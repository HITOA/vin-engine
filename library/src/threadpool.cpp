#include <vin/core/thread/threadpool.h>

Vin::Core::Thread::ThreadPool::ThreadPool()
{
    unsigned int threadCount = std::thread::hardware_concurrency();
    workers.resize(threadCount);
    for (unsigned int i = 0; i < threadCount; ++i) {
        workers[i] = std::thread{ std::bind(&ThreadPool::TheadLoop, this) };
    }
}

Vin::Core::Thread::ThreadPool::ThreadPool(unsigned int threadCount) {
    workers.resize(threadCount);
    for (unsigned int i = 0; i < threadCount; ++i) {
        workers[i] = std::thread{ std::bind(&ThreadPool::TheadLoop, this) };
    }
}

Vin::Core::Thread::ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock{ mutex };
        terminate = true;
    }

    cv.notify_all();
    for (auto& worker : workers)
        worker.join();

    workers.clear();
}

void Vin::Core::Thread::ThreadPool::Execute(const std::function<void()>& job)
{
    {
        std::unique_lock<std::mutex> lock{ mutex };
        jobs.push(job);
    }
    cv.notify_one();
}

unsigned int Vin::Core::Thread::ThreadPool::GetThreadCount() const
{
    return workers.size();
}

void Vin::Core::Thread::ThreadPool::TheadLoop()
{
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock{ mutex };
            cv.wait(lock, [&] {
                return !jobs.empty() || terminate;
            });
            if (terminate)
                return;
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}