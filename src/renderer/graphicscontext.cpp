#include "graphicscontext.hpp"

#include "core/assert.hpp"

#include "renderer/renderer.hpp"

#include "opengl/graphicscontext_opengl.hpp"

std::unique_ptr<Vin::GraphicsContext> Vin::GraphicsContext::Create(void* window)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return std::make_unique<Vin::OpenGLContext>((GLFWwindow*)window);
    }
}
