#ifndef VIN_ENGINE_STACKALLOCATOR_H
#define VIN_ENGINE_STACKALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>

namespace Vin::Core::Memory {

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
        inline Blk Allocate(size_t size) {
            size_t alignedSize = AlignForwardSize(size, Alignment);
            if (alignedSize > Size - offset)
                return { nullptr, 0 };
            void* ptr = buff + offset;
            offset += alignedSize;
            return { ptr, alignedSize };
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            if (buff + offset - blk.size != blk.ptr)
                return false;
            size_t alignedSize = AlignForwardSize(newSize, Alignment);
            if (offset + alignedSize - blk.size > Size)
                return false;
            blk.size = alignedSize;
            offset += alignedSize - blk.size;
            return true;
        }
        inline bool Deallocate(Blk& blk) {
            if (buff + offset - blk.size != blk.ptr)
                return false;
            offset -= blk.size;
            return true;
        }
        inline void Reset() {
            offset = 0;
        }
        inline bool Owns(Blk& blk) {
            return blk.ptr >= buff && blk.ptr < buff + Size;
        }

    private:
        char* buff{ nullptr };
        size_t offset{ 0 };
    };

}

#endif //VIN_ENGINE_STACKALLOCATOR_H
