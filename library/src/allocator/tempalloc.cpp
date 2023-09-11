#include <vin/allocator/tempalloc.h>
#include <cassert>
#include <cstdlib>

static bool isInitialized{ false };

static struct TempAllocatorData {
    char* blk{ nullptr };
    size_t blkSize{ 0 };
    size_t offset{ 0 };
} tempAllocatorData;

void Vin::Allocator::TempAllocator::Initialize(Vin::Allocator::TempAllocator::InitializationData data) {
    if (isInitialized)
        return;

    assert((data.blkSize > 0) && "Temp allocator block size can't be zero.");

    tempAllocatorData.blk = (char*)malloc(data.blkSize);
    tempAllocatorData.blkSize = data.blkSize;
    assert((tempAllocatorData.blk != nullptr) && "Failed to allocate memory block.");

    isInitialized = true;
}

void Vin::Allocator::TempAllocator::Uninitialize() {
    if (!isInitialized)
        return;

    free(tempAllocatorData.blk);
    tempAllocatorData = TempAllocatorData{};

    isInitialized = false;
}

void* Vin::Allocator::TempAllocator::Allocate(size_t size) {
    assert((tempAllocatorData.offset + size < tempAllocatorData.blkSize) && "Temp allocator out of space.");
    void* ptr = tempAllocatorData.blk + tempAllocatorData.offset;
    tempAllocatorData.offset += size;
    return ptr;
}

void Vin::Allocator::TempAllocator::Reset() {
    tempAllocatorData.offset = 0;
}