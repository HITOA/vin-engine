#include <vin/scene/resources/program.h>

Vin::Program::Program(Ref<Vin::Shader> vertexShader, Ref<Vin::Shader> fragmentShader) : vsh{ vertexShader }, fsh{ fragmentShader } {
    handle = bgfx::createProgram(vertexShader->GetShaderHandle(), fragmentShader->GetShaderHandle());
}

bgfx::ProgramHandle Vin::Program::GetProgramHandle() {
    return handle;
}