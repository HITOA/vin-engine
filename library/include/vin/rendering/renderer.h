#ifndef VIN_ENGINE_RENDERER_H
#define VIN_ENGINE_RENDERER_H

#define VIN_RENDERER_CONFIG_FLAG_MAILBOX_SWAPCHAIN 0x1
#define VIN_RENDERER_CONFIG_FLAG_ENABLE_STENCIL 0x2

#define VIN_INVALID_HANDLE 0xFFFFFFFF

#include <stddef.h>
#include <stdint.h>

namespace Vin {

    typedef unsigned int ShaderHandle;
    typedef unsigned int ProgramHandle;
    typedef unsigned int FramebufferHandle;

    struct RendererInitInfo {
        void* nativeWindowHandle{ nullptr };
        void* nativeDisplayHandle{ nullptr };
        uint32_t width{ 0 };
        uint32_t height{ 0 };
        uint32_t flags{ 0 };
    };

    class Renderer {
    public:
        static void Init(const RendererInitInfo& initInfo);
        static void Terminate();
        static void SyncRenderThread(); //Wait for the thread to be idle and ready to render a new frame.
        static Vin::ShaderHandle LoadShader(char* data, size_t size);
    };

}

#endif //VIN_ENGINE_RENDERER_H
