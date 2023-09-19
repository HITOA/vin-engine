#ifndef VIN_ENGINE_BUDDYALLOCATOR_H
#define VIN_ENGINE_BUDDYALLOCATOR_H

#include <stddef.h>
#include <stdlib.h>
#include <vin/core/memory/memutils.h>
#include <vin/core/memory/allocatordef.h>

#include <iostream>

namespace Vin::Core::Memory {

    struct BuddyBlock {
        size_t size{ 0 };
        bool isFree{ true };
    };

    template<size_t Size, size_t Alignment>
    class BuddyAllocator {
        static_assert(Size % Alignment == 0, "Size don't match alignment.");
        static_assert(sizeof(BuddyBlock) <= Alignment, "Alignment should be at least the size of a block.");

    public:
        BuddyAllocator() : buff{ (char*)malloc(Size) } {
            WriteBuddyBlock(buff, Size, true);
        }
        ~BuddyAllocator() {
            if (buff != nullptr)
                free(buff);
        }

    public:
        inline Blk Allocate(size_t size) {
            size_t bestSize = GetBuddyBlockBestSize(size);
            BuddyBlock* best = FindBestBuddyBlock((BuddyBlock*)buff, bestSize);
            if (best == nullptr) {
                BuddyBlockCoalescence();
                best = FindBestBuddyBlock((BuddyBlock*)buff, bestSize);
            }
            if (best != nullptr) {
                best->isFree = false;
                return { (void*)((char*)best + Alignment), bestSize - Alignment };
            }
            return { nullptr, 0 };
        }
        inline bool Reallocate(Blk& blk, size_t newSize) {
            //Todo
        }
        inline bool Deallocate(Blk& blk) {
            BuddyBlock* block = (BuddyBlock*)((char*)blk.ptr - Alignment);
            block->isFree = true;
            return true;
        }
        inline void Reset() {
            WriteBuddyBlock(buff, Size, true);
        }
        inline bool Owns(Blk& blk) {
            return blk.ptr >= buff && blk.ptr < buff + Size;
        }

    private:
        inline size_t GetBuddyBlockBestSize(size_t size) {
            size_t bestSize = Alignment;
            size = AlignForwardSize(size + Alignment, Alignment);
            while (size > bestSize)
                bestSize <<= 1;
            return bestSize;
        }
        inline void WriteBuddyBlock(void* ptr, size_t size, bool isFree) {
            BuddyBlock* block = (BuddyBlock*)ptr;
            block->size = size;
            block->isFree = isFree;
        }
        inline BuddyBlock* GetNextBuddyBlock(BuddyBlock* curr) {
            return (BuddyBlock*)((char*)curr + curr->size);
        }
        inline BuddyBlock* SplitBuddyBlock(BuddyBlock* curr, size_t size) {
            if (curr != nullptr && size != 0) {
                while (size < curr->size) {
                    size_t newSize = curr->size >> 1;
                    curr->size = newSize;
                    WriteBuddyBlock(GetNextBuddyBlock(curr), newSize, true);
                }

                if (size <= curr->size)
                    return curr;
            }
            return nullptr;
        }
        inline bool IsTail(BuddyBlock* curr) {
            return (void*)(curr) >= (void*)(buff + Size);
        }
        BuddyBlock* FindBestBuddyBlock(BuddyBlock* head, size_t size) {
            BuddyBlock* best = nullptr;
            BuddyBlock* block = head;
            BuddyBlock* buddy = GetNextBuddyBlock(block);

            if (IsTail(buddy) && block->isFree)
                return SplitBuddyBlock(block, size);

            while (!IsTail(block) && !IsTail(buddy)) {
                if (block->isFree && buddy->isFree && block->size == buddy->size) {
                    block->size <<= 1;
                    if (size <= block->size && (best == nullptr || block->size <= best->size))
                        best = block;
                    block = GetNextBuddyBlock(block);
                    if (!IsTail(block))
                        buddy = GetNextBuddyBlock(block);
                    continue;
                }

                if (block->isFree && size <= block->size && (best == nullptr || block->size <= best->size))
                    best = block;
                if (buddy->isFree && size <= buddy->size && (best == nullptr || buddy->size <= best->size))
                    best = buddy;

                if (block->size <= buddy->size) {
                    block = GetNextBuddyBlock(buddy);
                    if (!IsTail(block))
                        buddy = GetNextBuddyBlock(block);
                } else {
                    block = buddy;
                    buddy = GetNextBuddyBlock(buddy);
                }
            }

            if (best != nullptr)
                return SplitBuddyBlock(best, size);

            return nullptr;
        }
        inline void BuddyBlockCoalescence() {
            while (true) {
                BuddyBlock* block = (BuddyBlock*)buff;
                BuddyBlock* buddy = GetNextBuddyBlock(block);
                bool noCoalescence = true;
                while (!IsTail(block) && !IsTail(buddy)) {
                    if (block->isFree && buddy->isFree && block->size == buddy->size) {
                        block->size <<= 1;
                        block = GetNextBuddyBlock(block);
                        if (!IsTail(block)) {
                            buddy = GetNextBuddyBlock(block);
                            noCoalescence = false;
                        }
                    } else if (block->size < buddy->size) {
                        block = buddy;
                        buddy = GetNextBuddyBlock(buddy);
                    } else {
                        block = GetNextBuddyBlock(buddy);
                        if (!IsTail(block))
                            buddy = GetNextBuddyBlock(block);
                    }
                }

                if (noCoalescence)
                    return;
            }
        }

    private:
        char* buff{ nullptr };
    };

}

#endif //VIN_ENGINE_BUDDYALLOCATOR_H
