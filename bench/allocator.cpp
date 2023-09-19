#include "allocator.h"
#include <vin/core/memory/allocator.h>
#include <nanobench.h>

#define ALLOCATION_SIZE 64
#define ALLOCATION_COUNT 256

void AllocDeallocBenchmark() {
    ankerl::nanobench::Bench bench{};
    bench.name("Alloc Dealloc Benchmark");
    bench.relative(true);

    bench.epochIterations(256);

    Vin::Core::Memory::Mallocator mallocator{};
    Vin::Core::Memory::StackAllocator<ALLOCATION_SIZE * 2, 16> stackAllocator{};
    Vin::Core::Memory::BestFitAllocator<ALLOCATION_SIZE * 2, 16> bestFitAllocator{};
    Vin::Core::Memory::BuddyAllocator<ALLOCATION_SIZE * 2, 16> buddyAllocator{};

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

    bench.run("Best Fit Allocator", [&](){
        {
            Vin::Core::Memory::Blk blk = bestFitAllocator.Allocate(ALLOCATION_SIZE);
            bestFitAllocator.Deallocate(blk);
            bench.doNotOptimizeAway(blk);
        }
    });

    bench.run("Buddy Allocator", [&](){
        {
            Vin::Core::Memory::Blk blk = buddyAllocator.Allocate(ALLOCATION_SIZE);
            buddyAllocator.Deallocate(blk);
            bench.doNotOptimizeAway(blk);
        }
    });
}

void AllocResetBenchmark() {
    ankerl::nanobench::Bench bench{};
    bench.name("Alloc Reset Benchmark");
    bench.relative(true);

    bench.epochIterations(256);

    Vin::Core::Memory::Mallocator mallocator{};
    Vin::Core::Memory::StackAllocator<ALLOCATION_SIZE * ALLOCATION_COUNT * 2, 16> stackAllocator{};
    Vin::Core::Memory::BestFitAllocator<ALLOCATION_SIZE * ALLOCATION_COUNT * 2, 16> bestFitAllocator{};
    Vin::Core::Memory::BuddyAllocator<ALLOCATION_SIZE * ALLOCATION_COUNT * 2, 16> buddyAllocator{};

    bench.run("Mallocator", [&](){
        {
            static Vin::Core::Memory::Blk ptrs[ALLOCATION_COUNT]{};
            for (auto& ptr : ptrs) {
                ptr = mallocator.Allocate(ALLOCATION_SIZE);
            }
            for (auto& ptr : ptrs) {
                mallocator.Deallocate(ptr);
            }
        }
    });

    bench.run("Stack Allocator", [&](){
        {
            for (int i = 0; i < ALLOCATION_COUNT; ++i) {
                bench.doNotOptimizeAway(stackAllocator.Allocate(ALLOCATION_SIZE));
            }
            stackAllocator.Reset();
        }
    });

    bench.run("Best Fit Allocator", [&](){
        {
            for (int i = 0; i < ALLOCATION_COUNT; ++i) {
                bench.doNotOptimizeAway(bestFitAllocator.Allocate(ALLOCATION_SIZE));
            }
            bestFitAllocator.Reset();
        }
    });

    bench.run("Buddy Allocator", [&](){
        {
            for (int i = 0; i < ALLOCATION_COUNT; ++i) {
                bench.doNotOptimizeAway(buddyAllocator.Allocate(ALLOCATION_SIZE));
            }
            buddyAllocator.Reset();
        }
    });
}

void BenchmarkAllocator() {
    setbuf(stdout, 0);

    printf("Allocate Deallocate Benchmark : \n");
    AllocDeallocBenchmark();

    printf("Allocate Reset Benchmark : \n");
    AllocResetBenchmark();
}