#ifndef VIN_ENGINE_BUDDYALLOCATOR_H
#define VIN_ENGINE_BUDDYALLOCATOR_H

#include <cstdlib>
#include <cstddef>

namespace Vin::Core::Allocator {

    struct BuddyBlockHeader {
        size_t size{ 0 };
        bool isFree{ true };
    };

    class BuddyAllocator {
    public:
        BuddyAllocator() = delete;
        explicit BuddyAllocator(size_t blkSize, size_t alignment) {
            this->blk = (char*)malloc(blkSize);
            this->blkSize = blkSize;
            this->ownsBlk = true;
            this->alignment = alignment;
            WriteBlockHeader(this->blk, blkSize, true);
        }
        BuddyAllocator(char* blk, size_t blkSize, size_t alignment) {
            this->blk = blk;
            this->blkSize = blkSize;
            this->alignment = alignment;
            WriteBlockHeader(blk, blkSize, true);
        }
        ~BuddyAllocator() {
            if (ownsBlk)
                free(blk);
        }
    public:
        inline void* Allocate(size_t size) {
            size_t newSize = GetRequiredSize(size);
            BuddyBlockHeader* best = FindBestBuddyBlock((BuddyBlockHeader*)blk, newSize);
            if (best == nullptr) {
                BuddyBlockCoalescence();
                best = FindBestBuddyBlock((BuddyBlockHeader*)blk, newSize);
            }
            if (best != nullptr) {
                best->isFree = false;
                return (void*)((char*)best + alignment);
            }
            return nullptr;
        }
        inline void Free(void* ptr) {
            BuddyBlockHeader* block = (BuddyBlockHeader*)((char*)ptr - alignment);
            block->isFree = true;
            //BuddyBlockCoalescence();
        }
    private:
        inline void WriteBlockHeader(void* ptr, size_t size, bool isFree) {
            ((BuddyBlockHeader*)ptr)->size = size;
            ((BuddyBlockHeader*)ptr)->isFree = isFree;
        }
        inline BuddyBlockHeader* GetNextBuddyBlockHeader(BuddyBlockHeader* curr) {
            return (BuddyBlockHeader*)((char*)curr + curr->size);
        }
        inline BuddyBlockHeader* SplitBuddyBlockToFit(BuddyBlockHeader* curr, size_t size) {
            if (curr != nullptr) {
                while (size < curr->size) {
                    size_t newSize = curr->size >> 1;
                    curr->size = newSize;
                    WriteBlockHeader(GetNextBuddyBlockHeader(curr), newSize, true);
                }

                if (size <= curr->size)
                    return curr;
            }

            return nullptr;
        }
        inline bool IsTail(BuddyBlockHeader* curr) {
            return (void*)(curr) == (void*)(blk + blkSize);
        }
        BuddyBlockHeader* FindBestBuddyBlock(BuddyBlockHeader* head, size_t size) {
            BuddyBlockHeader* best = nullptr;
            BuddyBlockHeader* block = head;
            BuddyBlockHeader* buddy = GetNextBuddyBlockHeader(block);

            if (IsTail(buddy) && block->isFree)
                return SplitBuddyBlockToFit(block, size);

            while (!IsTail(block) && !IsTail(buddy)) {
                if (block->isFree && buddy->isFree && block->size == buddy->size) {
                    block->size <<= 1;
                    if (size <= block->size && (best == nullptr || block->size < best->size))
                        best = block;
                    block = GetNextBuddyBlockHeader(block);
                    if (!IsTail(block))
                        buddy = GetNextBuddyBlockHeader(block);
                    continue;
                }

                if (block->isFree && size <= block->size && (best == nullptr || block->size < best->size))
                    best = block;
                if (buddy->isFree && size <= buddy->size && (best == nullptr || buddy->size < best->size))
                    best = buddy;

                if (block->size <= buddy->size) {
                    block = GetNextBuddyBlockHeader(buddy);
                    if (!IsTail(block))
                        buddy = GetNextBuddyBlockHeader(block);
                } else {
                    block = buddy;
                    buddy = GetNextBuddyBlockHeader(buddy);
                }
            }

            if (best != nullptr)
                return SplitBuddyBlockToFit(best, size);

            return nullptr;
        }
        inline size_t GetRequiredSize(size_t size) {
            size_t rSize = alignment;
            size += sizeof(BuddyBlockHeader);
            while (size > rSize)
                rSize <<= 1;
            return rSize;
        }
        inline void BuddyBlockCoalescence() {
            while (1) {
                BuddyBlockHeader *block = (BuddyBlockHeader *) blk;
                BuddyBlockHeader *buddy = GetNextBuddyBlockHeader(block);
                bool noCoalescence = true;
                while (!IsTail(block) && !IsTail(buddy)) {
                    if (block->isFree && buddy->isFree && block->size == buddy->size) {
                        block->size <<= 1;
                        block = GetNextBuddyBlockHeader(block);
                        if (!IsTail(block)) {
                            buddy = GetNextBuddyBlockHeader(block);
                            noCoalescence = false;
                        }
                    } else if (block->size < buddy->size) {
                        block = buddy;
                        buddy = GetNextBuddyBlockHeader(buddy);
                    } else {
                        block = GetNextBuddyBlockHeader(buddy);
                        if (!IsTail(block))
                            buddy = GetNextBuddyBlockHeader(block);
                    }
                }

                if (noCoalescence)
                    return;
            }
        }
    private:
        char* blk{ nullptr };
        size_t blkSize{ 0 };
        bool ownsBlk{ false };
        size_t alignment{ 0 };
    };

}

#endif //VIN_ENGINE_BUDDYALLOCATOR_H
