#ifndef VIN_ENGINE_APPLICATION_H
#define VIN_ENGINE_APPLICATION_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/core/templates/ref.h>
#include <vin/core/templates/utils.h>
#include <vin/application/module.h>
#include <algorithm>
#include <tuple>

namespace Vin::Application {

    class App {
    public:
        void Run();

        template<typename T, typename... Args>
        inline void AddModule(Args... args) {
            Core::Ref<T> module{ Core::MakeRef<T>(args...) };
            InjectDependencies(module->dependencies);
            modules.emplace_back(module);
            modulesInfo.emplace_back(typeid(Core::Ref<T>));
        }

    private:
        inline void InjectDependencies(DependencyList<>&, size_t = 0) {}

        template<typename Dep>
        inline void InjectDependencies(DependencyList<Dep>& dependencies, size_t idx = 0) {
            static const std::type_index depInfo{ typeid(Core::Ref<Dep>) };
            auto it = std::find(modulesInfo.begin(), modulesInfo.end(), depInfo);
            if (it != modulesInfo.end()) {
                size_t mIdx = std::distance(modulesInfo.begin(), it);
                *(Core::Ref<Dep>*)(dependencies.fields[idx]) =
                        Core::Ref<Dep>{ modules[mIdx].GetRefData(), (Dep*)modules[mIdx].Get() };
            }
        }

        /*template<typename... Deps>
        inline void InjectDependencies(DependencyList<Deps...>& dependencies) {
            static const size_t depCount = sizeof...(Deps);
            static const std::type_index depsInfo[depCount]{ typeid(Core::Ref<Deps>)... };

            Core::ConstFor<depCount>([&](auto i){
                auto it = std::find(modulesInfo.begin(), modulesInfo.end(), depsInfo[i.value]);
                if (it != modulesInfo.end()) {
                    size_t idx = std::distance(modulesInfo.begin(), it);
                    *(Core::Ref<Core::GetVariadicTemplateType<i.value, Deps...>::type>*)(dependencies.fields[i.value]) =
                            Core::Ref<Core::GetVariadicTemplateType<i.value, Deps...>::type>{};
                }
            });
        }*/

    private:
        Core::Vector<Core::Ref<Module>> modules{};
        Core::Vector<std::type_index> modulesInfo{};
    };

}

#endif //VIN_ENGINE_APPLICATION_H
