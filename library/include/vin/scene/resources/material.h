#ifndef VIN_ENGINE_MATERIAL_H
#define VIN_ENGINE_MATERIAL_H

#include <vin/scene/resources/program.h>

namespace Vin {

    class Material {
    public:
        explicit Material(Ref<Program> program);

        bgfx::ProgramHandle GetProgramHandle();

    private:
        Ref<Program> program{};
        //Ref<Material> nextPass{}
    };

}

#endif //VIN_ENGINE_MATERIAL_H
