#ifndef VIN_ENGINE_RENDERER_H
#define VIN_ENGINE_RENDERER_H

#include <vin/core/templates/ref.h>

namespace Vin {

    struct RendererInitInfo {
        void* nativeWindowHandle;
        void* nativeDisplayHandle;
    };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual bool Init(const RendererInitInfo& initInfo) = 0;
        virtual void Terminate() = 0;
        virtual void Present() = 0;
    };

    class RenderingApi {
    public:
        static bool Init(const RendererInitInfo& initInfo);
        static void Terminate();
        static void Present();

    private:
        static Vin::Ref<Renderer> renderer;
    };

}

#endif //VIN_ENGINE_RENDERER_H
