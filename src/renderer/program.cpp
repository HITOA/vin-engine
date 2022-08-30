#include "program.hpp"

#include <assert.hpp>

#include "renderer/renderer.hpp"

#include "opengl/program_opengl.hpp"

eastl::shared_ptr<Vin::Program> Vin::Program::Create()
{
    VIN_ASSERT(Renderer::GetApi() != Renderer::None, "Rendering api not initialized.");

    switch (Renderer::GetApi()) {
    case Renderer::OpenGL: return eastl::make_shared<Vin::OpenGLProgram>();
    }
}
