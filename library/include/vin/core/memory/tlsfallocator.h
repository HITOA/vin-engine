#ifndef VIN_ENGINE_TLSFALLOCATOR_H
#define VIN_ENGINE_TLSFALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>
#include <tlsf.h>

#define MAX_TLSF_POOL_COUNT 32

namespace Vin::Core::Memory {

    /**
     * Two Level Segregated Fit Allocator
     */
    template<size_t PoolSize = 1024 * 1024>
    class TLSFAllocator {
    public:
        TLSFAllocator() {
            tlsf = malloc(tlsf_size());
            tlsf_create(tlsf);
            AddPool();
        }
        ~TLSFAllocator() {
            while (poolCount <= 0)
                PopPool();
            tlsf_destroy(tlsf);
            free(tlsf);
        }

    public:
        inline Blk Allocate(size_t size) {
            void* ptr = tlsf_malloc(tlsf, size);
            if (!ptr) {
                AddPool();
                ptr = tlsf_malloc(tlsf, size);
            }
            return { ptr, size };
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            blk.ptr = tlsf_realloc(tlsf, blk.ptr, newSize);
            blk.size = newSize;
        }
        inline bool Deallocate(Blk& blk) {
            tlsf_free(tlsf, blk.ptr);
            return true;
        }
        inline void Reset() {}
        inline bool Owns(Blk& blk) {
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
