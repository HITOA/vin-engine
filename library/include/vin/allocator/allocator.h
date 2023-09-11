#ifndef VIN_ENGINE_ALLOCATOR_H
#define VIN_ENGINE_ALLOCATOR_H

#include <vin/allocator/tempalloc.h>
#include <vin/allocator/persistentalloc.h>
#include <cstddef>

namespace Vin::Allocator {
    enum class Strategy {
        Temp,
        Persistent
    };

    template<Strategy strategy>
    inline void* Allocate(size_t size) { static_assert(false, "No allocation strategy given."); }
    template<Strategy strategy>
    inline void Free(void* ptr) { static_assert(false, "No allocation strategy given."); }

    template<>
    [[nodiscard]] inline void* Allocate<Strategy::Temp>(size_t size) {
        return TempAllocator::Allocate(size);
    }
    template<>
    inline void Free<Strategy::Temp>(void* ptr) {}

    template<>
    [[nodiscard]] inline void* Allocate<Strategy::Persistent>(size_t size) {
        return PersistentAllocator::Allocate(size);
    }
    template<>
    inline void Free<Strategy::Persistent>(void* ptr) {
        PersistentAllocator::Free(ptr);
    }
}

#endif //VIN_ENGINE_ALLOCATOR_H
