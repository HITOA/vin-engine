#ifndef VIN_ENGINE_MATERIAL_H
#define VIN_ENGINE_MATERIAL_H

#include <vin/scene/resources/program.h>

namespace Vin {

    class Material {
    public:
        explicit Material(Ref<Program> program);

    private:
        Ref<Program> program{};
    };

}

#endif //VIN_ENGINE_MATERIAL_H
