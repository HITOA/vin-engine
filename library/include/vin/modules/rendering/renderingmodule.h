#ifndef VIN_ENGINE_RENDERINGMODULE_H
#define VIN_ENGINE_RENDERINGMODULE_H

#include <vin/application/module.h>
#include <vin/modules/window/windowmodule.h>
#include <vin/core/templates/ref.h>
#include <vin/modules/rendering/renderpipeline.h>

namespace Vin::Modules {

    /**
     * BGFX RenderingApi
     */
    enum class RenderingApi {
        Noop,         //!< No rendering.
        Agc,          //!< AGC
        Direct3D9,    //!< Direct3D 9.0
        Direct3D11,   //!< Direct3D 11.0
        Direct3D12,   //!< Direct3D 12.0
        Gnm,          //!< GNM
        Metal,        //!< Metal
        Nvn,          //!< NVN
        OpenGLES,     //!< OpenGL ES 2.0+
        OpenGL,       //!< OpenGL 2.1+
        Vulkan,       //!< Vulkan
        WebGPU,       //!< WebGPU

        Count
    };

    class RenderingModule : public Module {
    public:
        DependencyList<WindowModule> dependencies{ windowModule };

        RenderingModule() = default;
        explicit RenderingModule(RenderingApi renderingApi);

        RenderingApi GetRenderingApi();

        void SetRenderPipeline(Ref<RenderPipeline> renderPipeline);
        Ref<RenderPipeline> GetRenderPipeline();

        void Initialize() final;
        void Uninitialize() final;

        void LateUpdate(TimeStep) final;

    private:
        int frameBufferWidth{}, frameBufferHeight{};
        Ref<WindowModule> windowModule{};
        Ref<RenderPipeline> renderPipeline{};
        RenderingApi renderingApi{ RenderingApi::Count };
    };

}

#endif //VIN_ENGINE_RENDERINGMODULE_H
