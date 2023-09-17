#ifndef VIN_ENGINE_MEMUTILS_H
#define VIN_ENGINE_MEMUTILS_H

#include <new>

namespace Vin::Core::Memory {

    template<class T, class... Args>
    inline void Construct(T* ptr, Args... args) {
        new (ptr)T(args...);
    }

    template<typename T>
    inline void Destroy(T* ptr) {
        ptr->~T();
    }

}

#endif //VIN_ENGINE_MEMUTILS_H
