#ifndef VIN_ENGINE_FRAMEWORKALLOCATOR_H
#define VIN_ENGINE_FRAMEWORKALLOCATOR_H

#include <cstddef>

#ifndef VIN_FRAMEWORK_ALLOCATOR_NO_IMPL
#include <vin/framework/frameworkmodule.h>
#include <vin/allocator/linearallocator.h>
#include <vin/allocator/generalallocator.h>
#endif

namespace Vin::Framework {

    enum class AllocationStrategy {
        Temp,
        Persistent
    };

    struct AllocatorFrameworkModuleInitializationData {
        size_t tempAllocatorMemoryBlockSize{ (size_t)3.2e7 };
        size_t persistentAllocatorMemoryBlockSize{ (size_t)3.2e8 };
    };

#ifndef VIN_FRAMEWORK_ALLOCATOR_NO_IMPL
    template<>
    class FrameworkModule<FrameworkModuleType::Allocator> {
    public:
        FrameworkModule() = delete;
        FrameworkModule(AllocatorFrameworkModuleInitializationData initializationData)
        : linearAllocator{initializationData.tempAllocatorMemoryBlockSize},
          generalAllocator{initializationData.persistentAllocatorMemoryBlockSize} {}
        ~FrameworkModule() = default;
    public:
        template<AllocationStrategy strategy>
        void* Allocate(size_t size) { return nullptr; }
        template<>
        void* Allocate<AllocationStrategy::Temp>(size_t size) { return linearAllocator.Allocate(size); }
        template<>
        void* Allocate<AllocationStrategy::Persistent>(size_t size) { return generalAllocator.Allocate(size); }

        template<AllocationStrategy strategy>
        void Free(void* ptr) {}
        template<>
        void Free<AllocationStrategy::Temp>(void* ptr) { linearAllocator.Free(ptr); }
        template<>
        void Free<AllocationStrategy::Persistent>(void* ptr) { generalAllocator.Free(ptr); }

    private:
        Allocator::LinearAllocator linearAllocator;
        Allocator::GeneralAllocator generalAllocator;
    };
#endif

}

#endif //VIN_ENGINE_FRAMEWORKALLOCATOR_H
