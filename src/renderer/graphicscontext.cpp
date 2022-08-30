#include "graphicscontext.hpp"

#include <assert.hpp>

#include "renderer/renderer.hpp"

#include "opengl/graphicscontext_opengl.hpp"

eastl::unique_ptr<Vin::GraphicsContext> Vin::GraphicsContext::Create(void* window)
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return eastl::make_unique<Vin::OpenGLContext>((GLFWwindow*)window);
    }
}
