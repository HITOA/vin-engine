#ifndef VIN_ENGINE_RENDERINGMODULE_H
#define VIN_ENGINE_RENDERINGMODULE_H

#include <vin/application/module.h>
#include <vin/modules/window/windowmodule.h>
#include <vin/core/templates/ref.h>

namespace Vin::Module {

    class RenderingModule : public Application::Module {
    public:
        Application::DependencyList<WindowModule> dependencies{ windowModule };

        void Initialize() final;
        void Uninitialize() final;

        void LateUpdate() final;

    private:
        int frameBufferWidth{}, frameBufferHeight{};
        Core::Ref<WindowModule> windowModule{};
    };

}

#endif //VIN_ENGINE_RENDERINGMODULE_H
