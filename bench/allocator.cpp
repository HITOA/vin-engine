#include "allocator.h"
#include <vin/core/memory/allocator.h>
#include <nanobench.h>

#define ALLOCATION_SIZE 64

void BenchmarkAllocator() {
    ankerl::nanobench::Bench bench{};
    bench.relative(true);

    bench.epochIterations(256);

    Vin::Core::Memory::Mallocator mallocator{};
    Vin::Core::Memory::StackAllocator<ALLOCATION_SIZE * 2, 16> stackAllocator{};

    bench.run("Mallocator", [&](){
        {
            Vin::Core::Memory::Blk blk = mallocator.Allocate(ALLOCATION_SIZE);
            mallocator.Deallocate(blk);
            bench.doNotOptimizeAway(blk);
        }
    });

    bench.run("Stack Allocator", [&](){
        {
            Vin::Core::Memory::Blk blk = stackAllocator.Allocate(ALLOCATION_SIZE);
            stackAllocator.Deallocate(blk);
            bench.doNotOptimizeAway(blk);
        }
    });

}