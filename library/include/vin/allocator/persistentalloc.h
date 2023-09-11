#ifndef VIN_ENGINE_PERSISTENTALLOC_H
#define VIN_ENGINE_PERSISTENTALLOC_H

#include <cstddef>

/**
 * Not implemented yet
 */
namespace Vin::Allocator::PersistentAllocator {

    [[nodiscard]] void* Allocate(size_t size);
    void Free(void* ptr);

}

#endif //VIN_ENGINE_PERSISTENTALLOC_H
