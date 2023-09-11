#ifndef VIN_ENGINE_TEMPALLOC_H
#define VIN_ENGINE_TEMPALLOC_H

#include <cstddef>

/**
 * Frame Based Linear Allocator
 */
namespace Vin::Allocator::TempAllocator {

    struct InitializationData {
        size_t blkSize{ 0 };
    };

    void Initialize(InitializationData data);
    void Uninitialize();

    [[nodiscard]] void* Allocate(size_t size);
    void Reset();

}

#endif //VIN_ENGINE_TEMPALLOC_H
