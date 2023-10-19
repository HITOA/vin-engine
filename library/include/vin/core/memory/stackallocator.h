#ifndef VIN_ENGINE_STACKALLOCATOR_H
#define VIN_ENGINE_STACKALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vin/core/memory/memutils.h>

namespace Vin::Core {

    template<size_t Size, size_t Alignment>
    class StackAllocator {
        static_assert(Size % Alignment == 0, "Size don't match alignment.");

    public:
        StackAllocator() : buff{ (char*)malloc(Size) }, offset{ 0 } {}
        ~StackAllocator() {
            if (buff != nullptr)
                free(buff);
        }

    public:
        inline void* Allocate(size_t size) {
            size_t alignedSize = AlignForwardSize(size, Alignment);
            if (alignedSize > Size - offset)
                return nullptr;
            void* ptr = buff + offset;
            offset += alignedSize;
            return ptr;
        }
        inline void* Reallocate(void*, size_t) { return nullptr; }
        inline void Deallocate(void*) {}
        inline void Reset() {
            offset = 0;
        }
        inline bool Owns(void* ptr) {
            return ptr >= buff && ptr < buff + Size;
        }

    private:
        char* buff{ nullptr };
        size_t offset{ 0 };
    };

}

#endif //VIN_ENGINE_STACKALLOCATOR_H
