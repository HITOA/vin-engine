#ifndef VIN_ENGINE_SEGREGATOR_H
#define VIN_ENGINE_SEGREGATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>

namespace Vin::Core::Memory {

    template<size_t Threshold, typename SmallAllocatorType, typename LargeAllocatorType>
    class Segregator {
    public:
        inline Blk Allocate(size_t size) {
            if (size <= Threshold)
                return smallAllocator.Allocate(size);
            return largeAllocator.Allocate(size);
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            if (smallAllocator.Owns(blk))
                return smallAllocator.Reallocate(blk, newSize);
            return largeAllocator.Reallocate(blk, newSize);
        }
        inline bool Deallocate(Blk& blk) {
            if (smallAllocator.Owns(blk))
                return smallAllocator.Deallocate(blk);
            return largeAllocator.Deallocate(blk);
        }
        inline void Reset() {
            smallAllocator.Reset();
            largeAllocator.Reset();
        }
        inline bool Owns(Blk& blk) {
            return smallAllocator.Owns(blk) || largeAllocator.Owns(blk);
        }
    private:
        SmallAllocatorType smallAllocator{};
        LargeAllocatorType largeAllocator{};
    };

}

#endif //VIN_ENGINE_SEGREGATOR_H
