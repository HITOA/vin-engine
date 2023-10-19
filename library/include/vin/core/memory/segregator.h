#ifndef VIN_ENGINE_SEGREGATOR_H
#define VIN_ENGINE_SEGREGATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>

namespace Vin::Core {

    template<size_t Threshold, typename SmallAllocatorType, typename LargeAllocatorType>
    class Segregator {
    public:
        inline void* Allocate(size_t size) {
            if (size <= Threshold)
                return smallAllocator.Allocate(size);
            return largeAllocator.Allocate(size);
        }
        inline bool Reallocate(void* ptr, size_t newSize) {
            if (smallAllocator.Owns(ptr))
                return smallAllocator.Reallocate(ptr, newSize);
            return largeAllocator.Reallocate(ptr, newSize);
        }
        inline void Deallocate(void* ptr) {
            if (smallAllocator.Owns(ptr))
                return smallAllocator.Deallocate(ptr);
            return largeAllocator.Deallocate(ptr);
        }
        inline void Reset() {
            smallAllocator.Reset();
            largeAllocator.Reset();
        }
        inline bool Owns(void* ptr) {
            return smallAllocator.Owns(ptr) || largeAllocator.Owns(ptr);
        }
    private:
        SmallAllocatorType smallAllocator{};
        LargeAllocatorType largeAllocator{};
    };

}

#endif //VIN_ENGINE_SEGREGATOR_H
