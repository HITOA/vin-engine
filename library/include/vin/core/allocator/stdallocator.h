#ifndef VIN_ENGINE_STDALLOCATOR_H
#define VIN_ENGINE_STDALLOCATOR_H

#include <cstddef>
#include <stdexcept>
#include <limits>

namespace Vin::Core::Allocator {

    template<class T, class A>
    class StdAllocator {
    public:
        using value_type = T;
    public:
        StdAllocator() = delete;
        explicit StdAllocator(A* allocator) {
            this->allocator = allocator;
        }
        template<class U>
        constexpr StdAllocator(const StdAllocator<U, A>& v) noexcept {
            allocator = v.allocator;
        }

        [[nodiscard]] T* allocate(size_t n) {
            if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
                throw std::bad_array_new_length();

            if (auto p = static_cast<T*>(allocator->Allocate(n * sizeof(T))))
                return p;

            throw std::bad_alloc();
        }

        void deallocate(T* ptr, size_t) {
            allocator->Free(ptr);
        }
    private:
        A* allocator{ nullptr };
    };

    template<class T, class U, class A>
    bool operator==(const StdAllocator<T, A>&, const StdAllocator<U, A>&) { return true; }

    template<class T, class U, class A>
    bool operator!=(const StdAllocator<T, A>&, const StdAllocator<U, A>&) { return false; }
}

#endif //VIN_ENGINE_STDALLOCATOR_H
