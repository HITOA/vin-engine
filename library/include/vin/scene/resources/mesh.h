#ifndef VIN_ENGINE_MESH_H
#define VIN_ENGINE_MESH_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>

namespace Vin {

    class Mesh {
    private:

    };

    template<>
    struct ResourceLoader<Mesh> {
        Ref<Mesh> operator()(const Ref<IO::File>& file) {
            return Vin::MakeRef<Mesh>();
        }
    };
}

#endif //VIN_ENGINE_MESH_H
