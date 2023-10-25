#ifndef VIN_ENGINE_STDALLOCATOR_H
#define VIN_ENGINE_STDALLOCATOR_H

#include <vin/core/memory/memorymanager.h>

namespace Vin::Core {

    template<class T, AllocationStrategy strategy>
    class VinStdAllocator {
    public:
        using value_type = T;

        VinStdAllocator() = default;
        template<class U>
        VinStdAllocator(const VinStdAllocator<U, strategy>&) noexcept {};

        [[nodiscard]] T* allocate(size_t n) {
            return (T*)MemoryManager::Allocate<strategy>(sizeof(T) * n);
        }

        void deallocate(void* ptr, size_t) {
            MemoryManager::Deallocate<strategy>(ptr);
        }

        template<class U>
        struct rebind {
            using other = VinStdAllocator<U, strategy>;
        };
    };

    template<class T, class U, Core::AllocationStrategy strategy>
    bool operator==(const VinStdAllocator<T, strategy>&, const VinStdAllocator<U, strategy>&) { return true; }

    template<class T, class U, Core::AllocationStrategy strategy>
    bool operator!=(const VinStdAllocator<T, strategy>&, const VinStdAllocator<U, strategy>&) { return false; }

}

#endif //VIN_ENGINE_STDALLOCATOR_H
