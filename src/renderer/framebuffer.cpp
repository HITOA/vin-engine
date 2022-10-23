#include "framebuffer.hpp"

#include "core/assert.hpp"

#include "renderer/renderer.hpp"

#include "opengl/framebuffer_opengl.hpp"

std::shared_ptr<Vin::RenderTarget> Vin::RenderTarget::Create(const RenderTargetSpecification& spec)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_shared<Vin::OpenGLRenderTarget>(spec);
    }

    return nullptr;
}

Vin::RenderBufferSpecification::RenderBufferSpecification(RenderBufferFormat format, bool isTexture)
    : format{ format }, isTexture{ isTexture } {}

Vin::RenderTargetSpecification::RenderTargetSpecification()
    : width{ 1 }, height{ 1 }, sample{ 1 }, attachements{} {}

Vin::RenderTargetSpecification::RenderTargetSpecification(uint32_t width, uint32_t height)
    : width{ width }, height{ height }, sample{ 1 }, attachements{} {}

Vin::RenderTargetSpecification::RenderTargetSpecification(uint32_t width, uint32_t height, uint32_t sample)
    : width{ width }, height{ height }, sample{ sample }, attachements{} {}

void Vin::RenderTargetSpecification::AddRenderBuffer(RenderBufferSpecification spec)
{
    attachements.push_back(spec);
}
