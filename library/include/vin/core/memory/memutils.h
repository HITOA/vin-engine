#ifndef VIN_ENGINE_MEMUTILS_H
#define VIN_ENGINE_MEMUTILS_H

#include <new>
#include <stddef.h>

namespace Vin::Core::Memory {

    template<typename T, typename... Args>
    inline void Construct(T* ptr, Args... args) {
        new(ptr)T(args...);
    }

    template<typename T>
    inline void Destroy(T* ptr) {
        ptr->~T();
    }

    inline size_t AlignForwardSize(size_t size, size_t alignment) {
        return size + (size % alignment);
    }

}

#endif //VIN_ENGINE_MEMUTILS_H
