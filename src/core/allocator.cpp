#include "allocator.hpp"

#include <mimalloc.h>

void* Vin::VinDefaultAllocator::AllocAligned(size_t size, size_t alignement)
{
    return mi_malloc_aligned(size, alignement);
}

void Vin::VinDefaultAllocator::FreeAligned(void* ptr, size_t alignement)
{
    mi_free_aligned(ptr, alignement);
}

void* Vin::VinDefaultAllocator::ReallocAligned(void* ptr, size_t size, size_t alignement)
{
    return mi_realloc_aligned(ptr, size, alignement);
}