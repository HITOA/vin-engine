#include <vin/scene/resources/material.h>

Vin::Material::Material(Ref<Vin::Program> program) : program{ program } {

}

bgfx::ProgramHandle Vin::Material::GetProgramHandle() {
    return program->GetProgramHandle();
}