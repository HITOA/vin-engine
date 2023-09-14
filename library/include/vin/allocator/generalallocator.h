#ifndef VIN_ENGINE_GENERALALLOCATOR_H
#define VIN_ENGINE_GENERALALLOCATOR_H

#include <cstdlib>
#include <cstddef>
#include <vector>

namespace Vin::Allocator {

    struct SlotHeader {
        size_t size{ 0 };
        char* next{ nullptr };
    };

    class GeneralAllocator {
    public:
        GeneralAllocator() = delete;
        explicit GeneralAllocator(size_t blkSize) {
            //this->blk = (char*)malloc(blkSize);
            //this->blkSize = blkSize;
        }
        ~GeneralAllocator() {
        }
    public:
        inline void* Allocate(size_t size) {
            return malloc(size);
        }
        inline void Free(void* ptr) {
            free(ptr);
        }
    };
}

#endif //VIN_ENGINE_GENERALALLOCATOR_H
