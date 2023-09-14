#ifndef VIN_ENGINE_LINEARALLOCATOR_H
#define VIN_ENGINE_LINEARALLOCATOR_H

#include <cstdlib>
#include <cstddef>

namespace Vin::Allocator {

    class LinearAllocator {
    public:
        LinearAllocator() = delete;
        explicit LinearAllocator(size_t blkSize) {
            this->blk = (char*)malloc(blkSize);
            this->blkSize = blkSize;
            this->offset = 0;
        }
        ~LinearAllocator() {
            free(blk);
        }
    public:
        inline void* Allocate(size_t size) {
            void* ptr = blk + offset;
            offset += size;
            return ptr;
        }
        inline void Free(void*) {}
        inline void Reset() {
            offset = 0;
        }
    private:
        char* blk{ nullptr };
        size_t blkSize{ 0 };
        size_t offset{ 0 };
    };
}

#endif //VIN_ENGINE_LINEARALLOCATOR_H
