#ifndef VIN_ENGINE_MODULE_H
#define VIN_ENGINE_MODULE_H

#include <vin/application/dependencylist.h>
#include <vin/time/timer.h>

namespace Vin {

    class Module {
    public:
        DependencyList<> dependencies{};

    public:
        virtual ~Module() {};

        virtual void Initialize() {};
        virtual void Uninitialize() {};

        virtual void EarlyUpdate(TimeStep) {};
        virtual void Update(TimeStep) {};
        virtual void LateUpdate(TimeStep) {};
    };

}

#endif //VIN_ENGINE_MODULE_H
