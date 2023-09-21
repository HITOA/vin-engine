#ifndef VIN_ENGINE_MEMORYMANAGER_H
#define VIN_ENGINE_MEMORYMANAGER_H

#include <vin/core/memory/allocator.h>
#include <vin/core/templates/singleton.h>

#define MEMORY_ALIGNMENT 16

namespace Vin::Core::Memory {

    using SingleFrameAllocator = StackAllocator<1 << 16, MEMORY_ALIGNMENT>;
    using DoubleFrameAllocator = DoubleBufferAllocator<StackAllocator<1 << 16, MEMORY_ALIGNMENT>>;
    using PersistentAllocator = TLSFAllocator<1024 * 1024>;
    //using PersistentAllocatorMemoryResource = TLSFMemoryResource<1024 * 1024>;

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
    class MemoryManager : public Singleton<MemoryManager> {
    public:
        inline void* Allocate(size_t size, Strategy strategy) {
            switch (strategy) {
                case Strategy::SingleFrame:
                    return singleFrameAllocator.Allocate(size);
                case Strategy::DoubleFrame:
                    return doubleFrameAllocator.Allocate(size);
                case Strategy::Persistent:
                    return persistentAllocator.Allocate(size);
                default:
                    return nullptr;
            }
        }
        inline void* Reallocate(void* ptr, size_t newSize, Strategy strategy) {
            switch (strategy) {
                case Strategy::SingleFrame:
                    return singleFrameAllocator.Reallocate(ptr, newSize);
                case Strategy::DoubleFrame:
                    return doubleFrameAllocator.Reallocate(ptr, newSize);
                case Strategy::Persistent:
                    return persistentAllocator.Reallocate(ptr, newSize);
                default:
                    return nullptr;
            }
        }
        inline void Deallocate(void* ptr, Strategy strategy) {
            switch (strategy) {
                case Strategy::SingleFrame:
                    return singleFrameAllocator.Deallocate(ptr);
                case Strategy::DoubleFrame:
                    return doubleFrameAllocator.Deallocate(ptr);
                case Strategy::Persistent:
                    return persistentAllocator.Deallocate(ptr);
                default:
                    return;
            }
        }

        template<Strategy strategy>
        inline void* Allocate(size_t size) {
            return Allocate(size, strategy);
        }

        template<Strategy strategy>
        inline void* Reallocate(void* ptr, size_t newSize) {
            return Reallocate(ptr, newSize, strategy);
        }

        template<Strategy strategy>
        inline void Deallocate(void* ptr) {
            return Deallocate(ptr, strategy);
        }

        /*inline auto GetMemoryResource() {
            return &persistentAllocatorMemoryResource;
        }*/

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
        //PersistentAllocatorMemoryResource persistentAllocatorMemoryResource{ &persistentAllocator };
    };

}

#endif //VIN_ENGINE_MEMORYMANAGER_H
