#ifndef VIN_ENGINE_RENDERINGMODULE_H
#define VIN_ENGINE_RENDERINGMODULE_H

#include <vin/application/module.h>
#include <vin/modules/window/windowmodule.h>
#include <vin/core/templates/ref.h>
#include <vin/modules/rendering/renderpipeline.h>

namespace Vin::Modules {

    class RenderingModule : public Module {
    public:
        DependencyList<WindowModule> dependencies{ windowModule };

        RenderingModule() = default;

        void SetRenderPipeline(Ref<RenderPipeline> renderPipeline);
        Ref<RenderPipeline> GetRenderPipeline();

        void Initialize() final;
        void Uninitialize() final;

        void LateUpdate(TimeStep) final;

    private:
        int frameBufferWidth{}, frameBufferHeight{};
        Ref<WindowModule> windowModule{};
        Ref<RenderPipeline> renderPipeline{};
    };

}

#endif //VIN_ENGINE_RENDERINGMODULE_H
