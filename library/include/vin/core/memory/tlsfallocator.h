#ifndef VIN_ENGINE_TLSFALLOCATOR_H
#define VIN_ENGINE_TLSFALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <tlsf.h>
#include <memory_resource>
#include <iostream>

#define MAX_TLSF_POOL_COUNT 32

namespace Vin::Core {

    /**
     * Two Level Segregated Fit Allocator
     */
    template<size_t PoolSize>
    class TLSFAllocator {
    public:
        TLSFAllocator() {
            tlsf = malloc(tlsf_size());
            tlsf_create(tlsf);
            AddPool();
        }
        ~TLSFAllocator() {
            for (size_t i = 0; i < poolCount; ++i)
                free(pools[i]);
            free(tlsf);
        }

    public:
        inline void* Allocate(size_t size) {
            void* ptr = tlsf_malloc(tlsf, size);
            if (!ptr) {
                AddPool();
                ptr = tlsf_malloc(tlsf, size);
            }
            return ptr;
        }
        inline void* Reallocate(void* ptr, size_t newSize) {
            void* newptr = tlsf_realloc(tlsf, ptr, newSize);;
            if (!newptr) {
                AddPool();
                newptr = tlsf_realloc(tlsf, ptr, newSize);;
            }
            return newptr;
        }
        inline void Deallocate(void* ptr) {
            tlsf_free(tlsf, ptr);
        }
        inline void Reset() {}
        inline bool Owns(void* ptr) {
            return true;
        }

    private:
        inline void AddPool() {
            pools[poolCount] = malloc(PoolSize);
            tlsf_add_pool(tlsf, pools[poolCount], PoolSize);
            ++poolCount;
        }
        inline void PopPool() {
            tlsf_remove_pool(tlsf, pools[--poolCount]);
            free(pools[poolCount]);
        }

    private:
        tlsf_t tlsf{ nullptr };
        pool_t pools[MAX_TLSF_POOL_COUNT]{};
        size_t poolCount{ 0 };
    };

}

#endif //VIN_ENGINE_TLSFALLOCATOR_H
