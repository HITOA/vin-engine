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
    typedef unsigned int TextureHandle;
    typedef unsigned int FramebufferHandle;

    struct RendererInitInfo {
        void* nativeWindowHandle{ nullptr };
        void* nativeDisplayHandle{ nullptr };
        uint32_t width{ 0 };
        uint32_t height{ 0 };
        uint32_t flags{ 0 };
    };

    struct FramebufferAttachmentDescription {
        enum Format {
            RGBA16F,    //HDR
            RGBA32F,    //HDR
            RGBA32,
            RGB24,
            RG16,
            R8,
            R16,
            R32,
            BGRA32
        };

        FramebufferAttachmentDescription::Format format{ RGBA32 };
        bool isDepthBuffer{ false };

    };

    struct FramebufferCreationInfo {
        uint32_t width{ 0 };
        uint32_t height{ 0 };
        uint32_t attachmentCount{ 0 };
        FramebufferAttachmentDescription attachments[8]{};

        inline void AddAttachment(FramebufferAttachmentDescription& description) {
            attachments[attachmentCount] = description;
            ++attachmentCount;
        }
    };

    class Renderer {
    public:
        static void Init(const RendererInitInfo& initInfo);
        static void Terminate();
        static void SyncRenderThread(); //Wait for the thread to be idle and ready to render a new frame.
        static Vin::ShaderHandle LoadShader(char* data, size_t size);
        static Vin::ProgramHandle CreateProgram(Vin::ShaderHandle vsh, Vin::ShaderHandle fsh);
        static Vin::FramebufferHandle CreateFramebuffer(const FramebufferCreationInfo& creationInfo);
        static Vin::TextureHandle GetFramebufferAttachment(Vin::FramebufferHandle framebuffer, uint32_t idx);
        static void SetRenderTarget(Vin::FramebufferHandle framebuffer); //Drawing operation will target this frame buffer
        static void Present(Vin::FramebufferHandle framebuffer); //Present framebuffer
    };

}

#endif //VIN_ENGINE_RENDERER_H
