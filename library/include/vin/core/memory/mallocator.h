//
// Created by HITO on 18/09/23.
//

#ifndef VIN_ENGINE_MALLOCATOR_H
#define VIN_ENGINE_MALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>

namespace Vin::Core::Memory {

    class Mallocator {
    public:
        inline Blk Allocate(size_t size) {
            return {malloc(size), size };
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            void* ptr = realloc(blk.ptr, newSize);
            if (ptr == nullptr)
                return false;
            blk.ptr = ptr;
            blk.size = newSize;
            return true;
        }
        inline bool Deallocate(Blk& blk) {
            free(blk.ptr);
            return true;
        }
    };

}

#endif //VIN_ENGINE_MALLOCATOR_H
