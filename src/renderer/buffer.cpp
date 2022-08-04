#include "buffer.hpp"

#include "core/assert.hpp"

#include "renderer/renderer.hpp"

#include "platform/opengl/buffer_opengl.hpp"

std::shared_ptr<Vin::VertexBuffer> Vin::VertexBuffer::Create(size_t size)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_shared<Vin::OpenGLVertexBuffer>(size);
    }
}

std::shared_ptr<Vin::IndexBuffer> Vin::IndexBuffer::Create(BufferIndexType type)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_shared<Vin::OpenGLIndexBuffer>(type);
    }
}
