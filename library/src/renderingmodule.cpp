#include <vin/modules/rendering/renderingmodule.h>
#include <vin/core/error/error.h>
#include <bgfx/bgfx.h>

void Vin::Module::RenderingModule::Initialize() {
    ASSERT(windowModule, "RenderingModule need WindowModule ton initialize.")

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

    frameBufferWidth = width;
    frameBufferHeight = height;

    windowModule->onFrameBufferResize += [this](int width, int height) {
        this->frameBufferWidth = width;
        this->frameBufferHeight = height;
        bgfx::reset(width, height, BGFX_RESET_VSYNC);
    };
}

void Vin::Module::RenderingModule::Uninitialize() {
    bgfx::shutdown();
}

void Vin::Module::RenderingModule::LateUpdate() {
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, frameBufferWidth, frameBufferHeight);
    bgfx::touch(0);
    bgfx::frame();
}