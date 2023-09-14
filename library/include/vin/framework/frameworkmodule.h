//
// Created by HITO on 13/09/23.
//

#ifndef VIN_ENGINE_FRAMEWORKMODULE_H
#define VIN_ENGINE_FRAMEWORKMODULE_H

namespace Vin::Framework {

    enum class FrameworkModuleType {
        None,
        Allocator,
        Rendering
    };

    template<FrameworkModuleType type>
    class FrameworkModule {
        static_assert(false, "Framework Module : No Implementation Provided");
    };

}

#endif //VIN_ENGINE_FRAMEWORKMODULE_H
