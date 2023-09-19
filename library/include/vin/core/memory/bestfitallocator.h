#ifndef VIN_ENGINE_BESTFITALLOCATOR_H
#define VIN_ENGINE_BESTFITALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>

namespace Vin::Core::Memory {

    struct BestFitAllocatorNode {
        size_t size{ 0 };
        BestFitAllocatorNode* next{ nullptr };
   };

    /**
     * Test allocator
     * Only keep track of the free memory with linked list (sorted by position)
     */
    template<size_t Size, size_t Alignment>
    class BestFitAllocator {
        static_assert(Size % Alignment == 0, "Size don't match alignment.");
        static_assert(sizeof(BestFitAllocatorNode) <= Alignment, "Alignment should be at least the size of a node.");

    public:
        BestFitAllocator() : buff{ (char*)malloc(Size) }, first{ (BestFitAllocatorNode*)buff } {
            first->size = Size;
            first->next = nullptr;
        }
        ~BestFitAllocator() {
            if (buff != nullptr)
                free(buff);
        }

    public:
        inline Blk Allocate(size_t size) {
            if (first == nullptr)
                return { nullptr, 0 };

            size_t alignedSize = AlignForwardSize(size, Alignment);

            BestFitAllocatorNode* curr{ first };
            BestFitAllocatorNode* best{ first };
            BestFitAllocatorNode* prev{ nullptr };
            while (curr != nullptr) {
                if (curr->size >= alignedSize && curr->size < best->size)
                    best = curr;
                if (best->size == alignedSize)
                    break;
                prev = curr;
                curr = curr->next;
            }

            if (best->size != alignedSize) {
                size_t rSize = best->size - alignedSize;
                if (rSize >= Alignment) {
                    BestFitAllocatorNode* newNode{ best + alignedSize };
                    newNode->size = rSize;
                    newNode->next = best->next;
                    best->next = newNode;
                    best->size = alignedSize;
                }
            }

            if (first == best) {
                if (best->next != nullptr) {
                    first = best->next;
                }
            } else {
                if (prev != nullptr)
                    prev->next = best->next;
            }

            return { best, best->size };
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            Blk newBlk = Allocate(newSize);
            if (newBlk.ptr == nullptr)
                return false;
            memcpy(newBlk.ptr, blk.ptr, blk.size);
            Deallocate(blk);
            blk.ptr = newBlk.ptr;
            blk.size = newBlk.size;
            return true;
        }
        inline bool Deallocate(Blk& blk) {
            if (first == nullptr) {
                first = (BestFitAllocatorNode*)blk.ptr;
                first->size = blk.size;
                first->next = nullptr;
                return true;
            }

            BestFitAllocatorNode* freeNode{ (BestFitAllocatorNode*)blk.ptr };
            BestFitAllocatorNode* prev{ first };
            while (prev->next < blk.ptr && prev->next != nullptr)
                prev = prev->next;
            freeNode->next = prev->next;
            prev->next = freeNode;

            //Merge Node
            if (freeNode + freeNode->size == freeNode->next) {
                freeNode->next = freeNode->next->next;
                freeNode->size += freeNode->next->size;
            }
            if (prev + prev->size == freeNode) {
                prev->next = freeNode->next;
                prev->size += freeNode->size;
            }

            return true;
        }
        inline void Reset() {
            first = (BestFitAllocatorNode*)buff;
            first->next = nullptr;
            first->size = Size;
        }
        inline bool Owns(Blk& blk) {
            return blk.ptr >= buff && blk.ptr < buff + Size;
        }

    private:
        char* buff{ nullptr };
        BestFitAllocatorNode* first{ nullptr };
    };

}

#endif //VIN_ENGINE_BESTFITALLOCATOR_H
