#include <vin/allocator/persistentalloc.h>
#include <cstdlib>

void* Vin::Allocator::PersistentAllocator::Allocate(size_t size) {
    return malloc(size);
}

void Vin::Allocator::PersistentAllocator::Free(void *ptr) {
    free(ptr);
}