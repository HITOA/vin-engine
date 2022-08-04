#include "vertexarray.hpp"

#include "core/assert.hpp"

#include "renderer/renderer.hpp"

#include "platform/opengl/vertexarray_opengl.hpp"

std::unique_ptr<Vin::VertexArray> Vin::VertexArray::Create()
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_unique<Vin::OpenGLVertexArray>();
    }
}
