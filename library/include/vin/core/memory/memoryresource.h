#ifndef VIN_ENGINE_MEMORYRESOURCE_H
#define VIN_ENGINE_MEMORYRESOURCE_H

#include <stddef.h>
#include <memory_resource>

namespace Vin::Core {

    template<typename Allocator>
    class MemoryResource : public std::pmr::memory_resource {
    public:
        explicit MemoryResource(Allocator* allocator) : allocator{ allocator } {}

    private:
        void* do_allocate( std::size_t size, std::size_t ) override {
            return allocator->Allocate(size);
        }
        void do_deallocate( void* p, std::size_t, std::size_t)  override {
            allocator->Deallocate(p);
        }
        [[nodiscard]] bool do_is_equal( const std::pmr::memory_resource&) const noexcept override {
            return true;
        }

    private:
        Allocator* allocator{ nullptr };
    };

}

#endif //VIN_ENGINE_MEMORYRESOURCE_H
