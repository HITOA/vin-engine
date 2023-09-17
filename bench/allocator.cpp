#include "allocator.h"
#include <vin/core/allocator/stdallocator.h>
#include <vin/core/allocator/linearallocator.h>
#include <vin/core/allocator/buddyallocator.h>
#include <vector>
#include <nanobench.h>

#define ITER_COUNT 8384

struct DummyData {
    int i{};
    char c[32]{};
};

void BenchmarkAllocator() {
    ankerl::nanobench::Bench bench{};
    bench.relative(true);

    bench.run("std::allocator with std container no reserve", [&](){
        {
            std::vector<DummyData, std::allocator<DummyData>> vec{};
            for (size_t i = 0; i < ITER_COUNT; ++i)
                vec.push_back(DummyData{});
        }
    });

    Vin::Core::Allocator::LinearAllocator linearAllocator{ (size_t)3.2e7 };

    bench.run("Vin::LinearAllocator with std container no reserve", [&](){
        {
            std::vector<DummyData, Vin::Core::Allocator::StdAllocator<DummyData, Vin::Core::Allocator::LinearAllocator>> vec{
                Vin::Core::Allocator::StdAllocator<DummyData, Vin::Core::Allocator::LinearAllocator>{ &linearAllocator } };
            for (size_t i = 0; i < ITER_COUNT; ++i)
                vec.push_back(DummyData{});
        }
        linearAllocator.Reset();
    });

    Vin::Core::Allocator::BuddyAllocator buddyAllocator{ 16 << 18, 16 };

    bench.run("Vin::BuddyAllocator with std container no reserve", [&](){
        {
            std::vector<DummyData, Vin::Core::Allocator::StdAllocator<DummyData, Vin::Core::Allocator::BuddyAllocator>> vec{
                    Vin::Core::Allocator::StdAllocator<DummyData, Vin::Core::Allocator::BuddyAllocator>{ &buddyAllocator } };
            for (size_t i = 0; i < ITER_COUNT; ++i)
                vec.push_back(DummyData{});
        }
    });
}