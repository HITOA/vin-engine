#ifndef VIN_ENGINE_MEMORYLOGGER_H
#define VIN_ENGINE_MEMORYLOGGER_H

#include <stddef.h>
#include <stdlib.h>
#include <iostream>

namespace Vin::Core {

    template<typename Allocator>
    class Logger {
    public:
        inline void* Allocate(size_t size) {
            void* ptr = allocator.Allocate(size);
            std::cout << "Allocate : " << ptr << ", " << size << std::endl;
            return ptr;
        }
        inline void* Reallocate(void* ptr, size_t newSize) {
            void* newptr = allocator.Reallocate(ptr, newSize);
            std::cout << "Reallocate : " << newptr << ", " << newSize << std::endl;
            return newptr;
        }
        inline void Deallocate(void* ptr) {
            std::cout << "Deallocate : " << ptr << std::endl;
            allocator.Deallocate(ptr);
        }
        inline void Reset() {}
        inline bool Owns(void*) {
            return true;
        }
    private:
        Allocator allocator{};
    };

}

#endif //VIN_ENGINE_MEMORYLOGGER_H
