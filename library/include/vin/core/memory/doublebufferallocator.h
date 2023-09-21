#ifndef VIN_ENGINE_DOUBLEBUFFERALLOCATOR_H
#define VIN_ENGINE_DOUBLEBUFFERALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>

namespace Vin::Core::Memory {

    /*
     * DoubleBufferAllocator<StackAllocator>
     */
    template<typename AllocatorType>
    class DoubleBufferAllocator {
    public:
        inline void* Allocate(size_t size) {
            if (aIsInFront)
                return allocatorA.Allocate(size);
            return allocatorB.Allocate(size);
        }
        inline void* Reallocate(void* ptr, size_t newSize) {
            if (aIsInFront)
                return allocatorA.Reallocate(ptr, newSize);
            return allocatorB.Reallocate(ptr, newSize);
        }
        /*
         * This function shouldn't be called
         */
        inline void Deallocate(void*) {}
        inline void Reset() {
            allocatorA.Reset();
            allocatorB.Reset();
        }
        inline bool Owns(void* ptr) {
            return allocatorA.Owns(ptr) || allocatorB.Owns(ptr);
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
