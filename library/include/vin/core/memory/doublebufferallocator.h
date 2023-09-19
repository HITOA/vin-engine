#ifndef VIN_ENGINE_DOUBLEBUFFERALLOCATOR_H
#define VIN_ENGINE_DOUBLEBUFFERALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>

namespace Vin::Core::Memory {

    /*
     * DoubleBufferAllocator<StackAllocator>
     */
    template<typename AllocatorType>
    class DoubleBufferAllocator {
    public:
        inline Blk Allocate(size_t size) {
            if (aIsInFront)
                return allocatorA.Allocate(size);
            return allocatorB.Allocate(size);
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            if (aIsInFront)
                return allocatorA.Reallocate(blk, newSize);
            return allocatorB.Reallocate(blk, newSize);
        }
        /*
         * This function shouldn't be called
         */
        inline bool Deallocate(Blk& blk) {
            return false;
        }
        inline void Reset() {
            allocatorA.Reset();
            allocatorB.Reset();
        }
        inline bool Owns(Blk& blk) {
            return allocatorA.Owns(blk) || allocatorB.Owns(blk);
        }
        /**
         * Swap front and back allocator, clear the previous back allocator
         */
        inline void Swap() {
            if (aIsInFront)
                allocatorB.Reset();
            else
                allocatorA.Reset();

            aIsInFront = !aIsInFront;
        }

    private:
        AllocatorType allocatorA{};
        AllocatorType allocatorB{};
        bool aIsInFront{ true };
    };

}

#endif //VIN_ENGINE_DOUBLEBUFFERALLOCATOR_H
