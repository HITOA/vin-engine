#include "vertexarray.hpp"

#include "core/assert.hpp"

#include "renderer/renderer.hpp"

#include "opengl/vertexarray_opengl.hpp"

std::shared_ptr<Vin::VertexArray> Vin::VertexArray::Create()
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_shared<Vin::OpenGLVertexArray>();
    }
}
