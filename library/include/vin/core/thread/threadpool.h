//
// Created by HITO on 15/09/23.
//

#ifndef VIN_ENGINE_THREADPOOL_H
#define VIN_ENGINE_THREADPOOL_H

#include <thread>
#include <functional>
#include <condition_variable>
#include <vin/core/templates/stdcontainers.h>

namespace Vin::Core {

    class ThreadPool {
    public:
        ThreadPool();
        ThreadPool(unsigned int threadCount);
        ~ThreadPool();

        void Execute(const std::function<void()>& job);
        unsigned int GetThreadCount() const;

    private:
        void TheadLoop();

    private:
        bool terminate{ false };
        std::mutex mutex{};
        std::condition_variable cv{};
        Vin::Vector<std::thread> workers{};
        Vin::Queue<std::function<void()>> jobs{};
    };

}

#endif //VIN_ENGINE_THREADPOOL_H
