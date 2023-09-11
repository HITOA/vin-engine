#ifndef VIN_ENGINE_STDALLOCATOR_H
#define VIN_ENGINE_STDALLOCATOR_H

#include <vin/allocator/allocator.h>
#include <cstdlib>
#include <stdexcept>
#include <limits>
/**
 * Implementation of the allocators for the standard template library.
 */
 namespace Vin::Allocator {

     template<class T>
     struct VinTempAllocator {
         typedef T value_type;

         VinTempAllocator() = default;

         template<class U>
         constexpr VinTempAllocator(const VinTempAllocator<U>&) noexcept {};

         [[nodiscard]] T* allocate(size_t n) {
             if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
                 throw std::bad_array_new_length();

             if (auto p = static_cast<T*>(Allocate<Strategy::Temp>(n * sizeof(T))))
                 return p;

             throw std::bad_alloc();
         }

         void deallocate(T* p, size_t n) noexcept {
             Free<Strategy::Temp>(p);
         }
     };

     template<class T, class U>
     bool operator==(const VinTempAllocator<T>&, const VinTempAllocator<U>&) { return true; }

     template<class T, class U>
     bool operator!=(const VinTempAllocator<T>&, const VinTempAllocator<U>&) { return false; }

     template<class T>
     struct VinPersistentAllocator {
         typedef T value_type;

         VinPersistentAllocator() = default;

         template<class U>
         constexpr VinPersistentAllocator(const VinPersistentAllocator<U>&) noexcept {};

         [[nodiscard]] T* allocate(size_t n) {
             if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
                 throw std::bad_array_new_length();

             if (auto p = static_cast<T*>(Allocate<Strategy::Persistent>(n * sizeof(T))))
                 return p;

             throw std::bad_alloc();
         }

         void deallocate(T* p, size_t n) noexcept {
             Free<Strategy::Persistent>(p);
         }
     };

     template<class T, class U>
     bool operator==(const VinPersistentAllocator<T>&, const VinPersistentAllocator<U>&) { return true; }

     template<class T, class U>
     bool operator!=(const VinPersistentAllocator<T>&, const VinPersistentAllocator<U>&) { return false; }

 }

#endif //VIN_ENGINE_STDALLOCATOR_H
