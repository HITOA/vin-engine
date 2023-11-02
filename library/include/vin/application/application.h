#ifndef VIN_ENGINE_APPLICATION_H
#define VIN_ENGINE_APPLICATION_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/core/templates/ref.h>
#include <vin/core/templates/utils.h>
#include <vin/application/module.h>
#include <algorithm>
#include <tuple>

namespace Vin {

    class App {
    public:
        void Run();
        void Loop();

        template<typename T, typename... Args>
        inline Ref<T> AddModule(Args... args) {
            Ref<T> module{ MakeRef<T>(args...) };
            InjectDependencies(module->dependencies);
            modules.emplace_back(module);
            modulesInfo.emplace_back(typeid(Ref<T>));
            return module;
        }

    private:
        template<typename Dep>
        inline void InjectDependencies(Ref<Dep>* field, std::type_index depInfo) {
            auto it = std::find(modulesInfo.begin(), modulesInfo.end(), depInfo);
            if (it != modulesInfo.end()) {
                size_t mIdx = std::distance(modulesInfo.begin(), it);
                *field = Ref<Dep>{ modules[mIdx].GetRefData(), (Dep*)modules[mIdx].Get() };
            }
        }

        template<typename... Deps, size_t... Is>
        inline void InjectDependencies(DependencyList<Deps...>& dependencies, std::index_sequence<Is...>) {
            int unused[] = { -1, (InjectDependencies((Ref<Deps>*)dependencies.fields[Is], typeid(Ref<Deps>)), 1)... };
            (void)unused;
        }

        template<typename... Deps>
        inline void InjectDependencies(DependencyList<Deps...>& dependencies) {
            InjectDependencies(dependencies, std::make_index_sequence<sizeof...(Deps)>());
        }

    private:
        Vector<Ref<Module>> modules{};
        Vector<std::type_index> modulesInfo{};
    };

}

#endif //VIN_ENGINE_APPLICATION_H
