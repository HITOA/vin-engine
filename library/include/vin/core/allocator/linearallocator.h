#ifndef VIN_ENGINE_LINEARALLOCATOR_H
#define VIN_ENGINE_LINEARALLOCATOR_H

#include <cstdlib>
#include <cstddef>

namespace Vin::Core::Allocator {

    class LinearAllocator {
    public:
        LinearAllocator() = delete;
        explicit LinearAllocator(size_t blkSize) {
            this->blk = (char*)malloc(blkSize);
            this->blkSize = blkSize;
            this->offset = 0;
            this->ownsBlk = true;
        }
        LinearAllocator(char* blk, size_t blkSize) {
            this->blk = blk;
            this->blkSize = blkSize;
            this->offset = 0;
        }
        ~LinearAllocator() {
            if (ownsBlk)
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
        bool ownsBlk{ false };
    };
}

#endif //VIN_ENGINE_LINEARALLOCATOR_H
