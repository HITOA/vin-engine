#include "buffer.hpp"

#include <assert.hpp>

#include "renderer/renderer.hpp"

#include "opengl/buffer_opengl.hpp"

eastl::shared_ptr<Vin::VertexBuffer> Vin::VertexBuffer::Create(size_t size)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return eastl::make_shared<Vin::OpenGLVertexBuffer>(size);
    }
}

eastl::shared_ptr<Vin::IndexBuffer> Vin::IndexBuffer::Create(BufferIndexType type)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return eastl::make_shared<Vin::OpenGLIndexBuffer>(type);
    }
}
