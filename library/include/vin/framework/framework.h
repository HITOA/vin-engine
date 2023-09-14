//
// Created by HITO on 13/09/23.
//

#ifndef VIN_ENGINE_FRAMEWORK_H
#define VIN_ENGINE_FRAMEWORK_H

#include <vin/framework/frameworkmodule.h>
#include <vin/framework/frameworkallocator.h>

namespace Vin::Framework {

    struct FrameworkInitializationData {
        AllocatorFrameworkModuleInitializationData allocatorInitializationData{};
    };

    class Framework {
    public:
        Framework() = delete;
        Framework(FrameworkInitializationData initializationData)
        : allocator{initializationData.allocatorInitializationData} {}
        ~Framework() = default;
    private:
        FrameworkModule<FrameworkModuleType::Allocator> allocator;
    };

}

#endif //VIN_ENGINE_FRAMEWORK_H
