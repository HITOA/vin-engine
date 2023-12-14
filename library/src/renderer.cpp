#include <vin/rendering/renderer.h>
#include <vin/rendering/vulkanrenderer.h>

Vin::Ref<Vin::Renderer> Vin::RenderingApi::renderer{};

bool Vin::RenderingApi::Init(const Vin::RendererInitInfo &initInfo) {
    if (!renderer)
        renderer = Vin::MakeRef<VulkanRenderer>();

    return renderer->Init(initInfo);
}

void Vin::RenderingApi::Terminate() {
    renderer->Terminate();
}

void Vin::RenderingApi::Present() {
    renderer->Present();
}