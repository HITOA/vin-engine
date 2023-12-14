#ifndef VIN_ENGINE_CONCURRENTCIRCULARQUEUE_H
#define VIN_ENGINE_CONCURRENTCIRCULARQUEUE_H

#include <stddef.h>
#include <atomic>
#include <mutex>
#include <vin/core/memory/memutils.h>
#include <vin/core/templates/containerbase.h>

namespace Vin {

    template<typename T, size_t size>
    class ConcurrentCircularQueue {
    public:
        ContainerStatus TryPush(const T& v) {
            std::lock_guard<std::mutex> guard{ bufferMutex };
            if (count >= size)
                return ContainerStatus::Full;

            buffer[(cursor + count) % size] = v;
            ++count;
            return ContainerStatus::Success;
        }
        ContainerStatus TryPop(T& v) {
            std::lock_guard<std::mutex> guard{ bufferMutex };
            if (count <= 0)
                return ContainerStatus::Empty;

            v = buffer[cursor];
            Core::Destroy(&buffer[cursor]);
            cursor = (cursor + 1) % size;
            --count;
            return ContainerStatus::Success;
        }
        [[nodiscard]] inline bool IsEmpty() const {
            return count <= 0;
        }
        [[nodiscard]] inline bool IsFull() const {
            return count >= size;
        }

    private:
        size_t cursor{ 0 };
        std::atomic<size_t> count{ 0 };
        std::mutex bufferMutex{};
        T buffer[size];
    };

}

#endif //VIN_ENGINE_CONCURRENTCIRCULARQUEUE_H
