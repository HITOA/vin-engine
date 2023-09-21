#ifndef VIN_ENGINE_TLSFALLOCATOR_H
#define VIN_ENGINE_TLSFALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <tlsf.h>
#include <memory_resource>

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
        inline void* Allocate(size_t size) {
            void* ptr = tlsf_malloc(tlsf, size);
            if (!ptr) {
                AddPool();
                ptr = tlsf_malloc(tlsf, size);
            }
            return ptr;
        }
        inline void* Reallocate(void* ptr, size_t newSize) {
            return tlsf_realloc(tlsf, ptr, newSize);
        }
        inline void Deallocate(void* ptr) {
            tlsf_free(tlsf, ptr);
        }
        inline void Reset() {}
        inline bool Owns(void* ptr) {
            return true;
        }

        inline void* AllocateAlligned(size_t size, size_t alignment) {
            void* ptr = tlsf_memalign(tlsf, alignment, size);
            if (!ptr) {
                AddPool();
                ptr = tlsf_memalign(tlsf, alignment, size);
            }
            return ptr;
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

    /*template<size_t PoolSize = 1024 * 1024>
    class TLSFMemoryResource : public std::pmr::memory_resource {
    public:
        explicit TLSFMemoryResource(TLSFAllocator<PoolSize>* allocator) : allocator{ allocator } {}

    private:
        void* do_allocate( std::size_t size, std::size_t alignment ) override {
            return allocator->AllocateAlligned(size, alignment);
        }
        void do_deallocate( void* p, std::size_t size, std::size_t alignment ) override {
            allocator->Deallocate({ p, 0 });
        }
        [[nodiscard]] bool do_is_equal( const std::pmr::memory_resource& other ) const noexcept override {
            TLSFMemoryResource<PoolSize>* o = dynamic_cast<TLSFMemoryResource<PoolSize>*>(&other);
            if (!o)
                return false;
            return o->allocator == allocator;
        }

    private:
        TLSFAllocator<PoolSize>* allocator{ nullptr };
    };*/

}

#endif //VIN_ENGINE_TLSFALLOCATOR_H
