#ifndef VIN_ENGINE_PROGRAM_H
#define VIN_ENGINE_PROGRAM_H

#include <vin/scene/resources/shader.h>
#include <bgfx/bgfx.h>

namespace Vin {

    class Program {
    public:
        Program(Ref<Shader> vertexShader, Ref<Shader> fragmentShader);

        bgfx::ProgramHandle GetProgramHandle();

    private:
        Ref<Shader> vsh{};
        Ref<Shader> fsh{};
        bgfx::ProgramHandle handle{};
    };

    //Todo: Program Resource Loader

}

#endif //VIN_ENGINE_PROGRAM_H
