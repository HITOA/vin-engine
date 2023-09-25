#ifndef VIN_ENGINE_DEPENDENCYLIST_H
#define VIN_ENGINE_DEPENDENCYLIST_H

#include <stddef.h>
#include <typeindex>
#include <vin/core/templates/ref.h>

namespace Vin::Application {

    template<typename... Ts>
    struct DependencyList {
        void* fields[sizeof...(Ts)]{};
        DependencyList(Core::Ref<Ts>&... args) : fields{ &args... } {}
    };

}

#endif //VIN_ENGINE_DEPENDENCYLIST_H
