#include <vin/modules/rendering/renderingmodule.h>
#include <vin/core/error/error.h>
#include <bgfx/bgfx.h>

void Vin::Module::RenderingModule::Initialize() {
    ASSERT(windowModule, "")

    int width{}, height{};
    windowModule->GetFrameBufferSize(width, height);

    bgfx::Init init;
    init.platformData.nwh = windowModule->GetNativeWindowHandle();
    init.platformData.ndt = windowModule->GetNativeDisplayHandle();

#if defined(VIN_LINUX) && defined(VIN_USE_WAYLAND)
    init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
#endif

    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = bgfx::RendererType::Vulkan;
    init.vendorId = 0;
    bgfx::init(init);
}

void Vin::Module::RenderingModule::Uninitialize() {
    bgfx::shutdown();
}