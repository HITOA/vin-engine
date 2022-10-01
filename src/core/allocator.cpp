#include "allocator.hpp"

#include <mimalloc.h>

void* __cdecl operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
}
void* __cdecl operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
};

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