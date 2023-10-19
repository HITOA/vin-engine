#ifndef VIN_ENGINE_MEMORYMANAGER_H
#define VIN_ENGINE_MEMORYMANAGER_H

#include <vin/core/memory/allocator.h>
#include <vin/core/memory/memoryresource.h>

#define MEMORY_ALIGNMENT 16

namespace Vin::Core {

    using SingleFrameAllocator = StackAllocator<1 << 16, MEMORY_ALIGNMENT>;
    using DoubleFrameAllocator = DoubleBufferAllocator<StackAllocator<1 << 16, MEMORY_ALIGNMENT>>;
    using PersistentAllocator = TLSFAllocator<>;

    enum class AllocationStrategy {
        None = 0,
        SingleFrame, //Allocate Through Single Frame (Stack) Allocator
        DoubleFrame, //Allocate Through Double Buffer (2xStack) Allocator
        Persistent, //Allocate Through The Main Allocator
        Shared //Allocate Through Thread Safe Allocator (Not Implemented Yet)
    };

    /**
     * MemoryManager
     * If you don't know wich strategy to use :
     *      - SingleFrame If You Don't Need The Memory The Next Frame (usefull for temporary object)
     *      - DoubleFrame If You Don't Need The Memory In Two Frame (usefull for temporary object that need to be passed to the next frame)
     *      - Persistent If You Need Persistent Memory That You Will Manually Deallocate. Two Level Segregate Allocator.
     *      - Shared If You Need To Pass Object To Another Thread
     */

    class MemoryManager {
    public:
        template<AllocationStrategy strategy>
        static inline void* Allocate(size_t size) {
            switch (strategy) {
                case AllocationStrategy::SingleFrame:
                    return singleFrameAllocator.Allocate(size);
                case AllocationStrategy::DoubleFrame:
                    return doubleFrameAllocator.Allocate(size);
                case AllocationStrategy::Persistent:
                    return persistentAllocator.Allocate(size);
                default:
                    return nullptr;
            }
        }

        template<AllocationStrategy strategy>
        static inline void* Reallocate(void* ptr, size_t newSize) {
            switch (strategy) {
                case AllocationStrategy::SingleFrame:
                    return singleFrameAllocator.Reallocate(ptr, newSize);
                case AllocationStrategy::DoubleFrame:
                    return doubleFrameAllocator.Reallocate(ptr, newSize);
                case AllocationStrategy::Persistent:
                    return persistentAllocator.Reallocate(ptr, newSize);
                default:
                    return nullptr;
            }
        }

        template<AllocationStrategy strategy>
        static inline void Deallocate(void* ptr) {
            switch (strategy) {
                case AllocationStrategy::SingleFrame:
                    return singleFrameAllocator.Deallocate(ptr);
                case AllocationStrategy::DoubleFrame:
                    return doubleFrameAllocator.Deallocate(ptr);
                case AllocationStrategy::Persistent:
                    return persistentAllocator.Deallocate(ptr);
                default:
                    return;
            }
        }

        template<AllocationStrategy strategy>
        static inline std::pmr::memory_resource* GetMemoryResource() {
            switch (strategy) {
                case AllocationStrategy::SingleFrame:
                    return &singleFrameMemoryResource;
                case AllocationStrategy::DoubleFrame:
                    return &doubleFrameMemoryResource;
                case AllocationStrategy::Persistent:
                    return &persistentMemoryResource;
                default:
                    return nullptr;
            }
        }

        /**
         * Swap Double Frame Allocator and Reset Single Frame Allocator
         */
        static inline void SwapAndReset() {
            doubleFrameAllocator.Swap();
            singleFrameAllocator.Reset();
        }

    private:
        static SingleFrameAllocator singleFrameAllocator;
        static DoubleFrameAllocator doubleFrameAllocator;
        static PersistentAllocator persistentAllocator;

        static MemoryResource<SingleFrameAllocator> singleFrameMemoryResource;
        static MemoryResource<DoubleFrameAllocator> doubleFrameMemoryResource;
        static MemoryResource<PersistentAllocator> persistentMemoryResource;
    };
}

#endif //VIN_ENGINE_MEMORYMANAGER_H
