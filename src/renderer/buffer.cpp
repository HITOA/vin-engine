#include "buffer.hpp"

#include "core/assert.hpp"

#include "renderer/renderer.hpp"

#include "platform/opengl/buffer_opengl.hpp"

std::unique_ptr<Vin::VertexBuffer> Vin::VertexBuffer::Create(size_t size)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_unique<Vin::OpenGLVertexBuffer>(size);
    }
}
