#include <vin/modules/rendering/renderingmodule.h>
#include <vin/core/error/error.h>
#include <vin/rendering/renderer.h>

void Vin::Modules::RenderingModule::SetRenderPipeline(Ref<RenderPipeline> renderPipeline) {
    this->renderPipeline = renderPipeline;
}

Vin::Ref<Vin::Modules::RenderPipeline> Vin::Modules::RenderingModule::GetRenderPipeline() {
    return renderPipeline;
}

void Vin::Modules::RenderingModule::Initialize() {
    ASSERT(windowModule, "RenderingModule need WindowModule ton initialize.")

    int width{}, height{};
    windowModule->GetFrameBufferSize(width, height);

    RendererInitInfo initInfo{};
    initInfo.nativeWindowHandle = windowModule->GetNativeWindowHandle();
    initInfo.nativeDisplayHandle = windowModule->GetNativeDisplayHandle();

    RenderingApi::Init(initInfo);

    /*bgfx::Init init;
    init.platformData.nwh = windowModule->GetNativeWindowHandle();
    init.platformData.ndt = windowModule->GetNativeDisplayHandle();

#if defined(VIN_LINUX) && defined(VIN_USE_WAYLAND)
    init.platformData.type = bgfx::NativeWindowHandleType::Wayland;
#endif

    init.resolution.width = width;
    init.resolution.height = height;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.type = (bgfx::RendererType::Enum)renderingApi;
    init.vendorId = 0;
    init.allocator = nullptr;
    bgfx::init(init);

    frameBufferWidth = width;
    frameBufferHeight = height;

    windowModule->onFrameBufferResize += [this](int width, int height) {
        this->frameBufferWidth = width;
        this->frameBufferHeight = height;
        bgfx::reset(width, height, BGFX_RESET_VSYNC);
    };*/
}

void Vin::Modules::RenderingModule::Uninitialize() {
    //bgfx::shutdown();
    RenderingApi::Terminate();
}

void Vin::Modules::RenderingModule::LateUpdate(TimeStep) {
    /*bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x443355FF, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, frameBufferWidth, frameBufferHeight);

    //if (renderPipeline)
        //renderPipeline->Render();

    bgfx::touch(0);
    bgfx::frame();*/
    RenderingApi::Present();
}