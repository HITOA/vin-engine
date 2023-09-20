#ifndef VIN_ENGINE_MEMORYMANAGER_H
#define VIN_ENGINE_MEMORYMANAGER_H

#include <vin/core/memory/allocator.h>

#define MEMORY_ALIGNMENT 16

namespace Vin::Core::Memory {

    using SingleFrameAllocator = StackAllocator<1 << 16, MEMORY_ALIGNMENT>;
    using DoubleFrameAllocator = DoubleBufferAllocator<StackAllocator<1 << 16, MEMORY_ALIGNMENT>>;
    using PersistentAllocator = TLSFAllocator<1024 * 1024>;

    enum class Strategy {
        None,
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
        template<Strategy strategy>
        inline Blk Allocate(size_t size) { return { nullptr, 0 }; }
        template<Strategy strategy>
        inline bool Reallocate(Blk& blk, size_t newSize) { return false; }
        template<Strategy strategy>
        inline bool Deallocate(Blk& blk) { return false; }

        template<>
        inline Blk Allocate<Strategy::SingleFrame>(size_t size) {
            return singleFrameAllocator.Allocate(size);
        }
        template<>
        inline bool Reallocate<Strategy::SingleFrame>(Blk& blk, size_t newSize) {
            return singleFrameAllocator.Reallocate(blk, newSize);
        }
        template<>
        inline bool Deallocate<Strategy::SingleFrame>(Blk& blk) {
            return singleFrameAllocator.Deallocate(blk);
        }

        template<>
        inline Blk Allocate<Strategy::DoubleFrame>(size_t size) {
            return doubleFrameAllocator.Allocate(size);
        }
        template<>
        inline bool Reallocate<Strategy::DoubleFrame>(Blk& blk, size_t newSize) {
            return doubleFrameAllocator.Reallocate(blk, newSize);
        }
        template<>
        inline bool Deallocate<Strategy::DoubleFrame>(Blk& blk) {
            return doubleFrameAllocator.Deallocate(blk);
        }

        template<>
        inline Blk Allocate<Strategy::Persistent>(size_t size) {
            return persistentAllocator.Allocate(size);
        }
        template<>
        inline bool Reallocate<Strategy::Persistent>(Blk& blk, size_t newSize) {
            return persistentAllocator.Reallocate(blk, newSize);
        }
        template<>
        inline bool Deallocate<Strategy::Persistent>(Blk& blk) {
            return persistentAllocator.Deallocate(blk);
        }

        template<>
        inline Blk Allocate<Strategy::Shared>(size_t size) {
            static_assert(false, "Shared Allocator Not Implemented Yet.");
        }
        template<>
        inline bool Reallocate<Strategy::Shared>(Blk& blk, size_t newSize) {
            static_assert(false, "Shared Allocator Not Implemented Yet.");
        }
        template<>
        inline bool Deallocate<Strategy::Shared>(Blk& blk) {
            static_assert(false, "Shared Allocator Not Implemented Yet.");
        }

        /**
         * Swap Double Frame Allocator and Reset Single Frame Allocator
         */
        inline void SwapAndReset() {
            doubleFrameAllocator.Swap();
            singleFrameAllocator.Reset();
        }

    private:
        SingleFrameAllocator singleFrameAllocator{};
        DoubleFrameAllocator doubleFrameAllocator{};
        PersistentAllocator persistentAllocator{};
    };

}

#endif //VIN_ENGINE_MEMORYMANAGER_H
