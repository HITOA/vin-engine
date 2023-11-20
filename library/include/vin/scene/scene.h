#ifndef VIN_ENGINE_SCENE_H
#define VIN_ENGINE_SCENE_H

#include <vin/entt/entt.h>
#include <vin/core/templates/ref.h>
#include <vin/core/templates/stdcontainers.h>

namespace Vin {

    class Scene {
    public:
        inline EnTT::registry* operator->() {
            return &registry;
        }

    private:
        EnTT::registry registry{};
    };

}

#endif
