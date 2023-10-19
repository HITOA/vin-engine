//
// Created by HITO on 18/09/23.
//

#ifndef VIN_ENGINE_MALLOCATOR_H
#define VIN_ENGINE_MALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>

namespace Vin::Core {

    class Mallocator {
    public:
        inline void* Allocate(size_t size) {
            return malloc(size);
        }
        inline void* Reallocate(void* ptr, size_t newSize) {
            return realloc(ptr, newSize);
        }
        inline void Deallocate(void* ptr) {
            free(ptr);
        }
        inline void Reset() {}
        inline bool Owns(void*) {
            return true;
        }
    };

}

#endif //VIN_ENGINE_MALLOCATOR_H
