#include <vin/core/memory/memorymanager.h>

namespace Vin::Core::Memory {

    SingleFrameAllocator MemoryManager::singleFrameAllocator{};
    DoubleFrameAllocator MemoryManager::doubleFrameAllocator{};
    PersistentAllocator MemoryManager::persistentAllocator{};

    MemoryResource<SingleFrameAllocator> MemoryManager::singleFrameMemoryResource{ &MemoryManager::singleFrameAllocator };
    MemoryResource<DoubleFrameAllocator> MemoryManager::doubleFrameMemoryResource{ &MemoryManager::doubleFrameAllocator };
    MemoryResource<PersistentAllocator> MemoryManager::persistentMemoryResource{ &MemoryManager::persistentAllocator };

}