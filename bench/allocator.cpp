#include "allocator.h"
#include <vin/allocator/stdallocator.h>
#include <vin/allocator/linearallocator.h>
#include <vin/allocator/generalallocator.h>
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

    Vin::Allocator::LinearAllocator linearAllocator{ (size_t)3.2e7 };

    bench.run("Vin::LinearAllocator with std container no reserve", [&](){
        {
            std::vector<DummyData, Vin::Allocator::StdAllocator<DummyData, Vin::Allocator::LinearAllocator>> vec{
                Vin::Allocator::StdAllocator<DummyData, Vin::Allocator::LinearAllocator>{ &linearAllocator } };
            for (size_t i = 0; i < ITER_COUNT; ++i)
                vec.push_back(DummyData{});
        }
        linearAllocator.Reset();
    });

    Vin::Allocator::GeneralAllocator generalAllocator{ (size_t)3.2e7 };


    bench.run("Vin::GeneralAllocator with std container no reserve", [&](){
        {
            std::vector<DummyData, Vin::Allocator::StdAllocator<DummyData, Vin::Allocator::GeneralAllocator>> vec{
                    Vin::Allocator::StdAllocator<DummyData, Vin::Allocator::GeneralAllocator>{ &generalAllocator } };
            for (size_t i = 0; i < ITER_COUNT; ++i)
                vec.push_back(DummyData{});
        }
        linearAllocator.Reset();
    });
}