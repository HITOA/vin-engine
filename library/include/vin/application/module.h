#ifndef VIN_ENGINE_MODULE_H
#define VIN_ENGINE_MODULE_H

#include <vin/application/dependencylist.h>

namespace Vin::Application {

    class Module {
    public:
        DependencyList<> dependencies{};

    public:
        virtual ~Module() {};

        virtual void Initialize() {};
        virtual void Uninitialize() {};

        virtual void EarlyUpdate() {};
        virtual void Update() {};
        virtual void LateUpdate() {};
    };

}

#endif //VIN_ENGINE_MODULE_H
