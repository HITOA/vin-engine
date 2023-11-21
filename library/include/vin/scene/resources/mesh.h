#ifndef VIN_ENGINE_MESH_H
#define VIN_ENGINE_MESH_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <bgfx/bgfx.h>

namespace Vin {

    struct Primitive {
        uint32_t startVertex;
        uint32_t startIndex;
        uint32_t numVertices;
        uint32_t numIndices;
    };

    template<Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    struct MeshData {
        bgfx::VertexLayout vertexLayout{};
        Vector<char, strategy> vertices{};
        Vector<uint32_t, strategy> indices{};
        Vector<Primitive, strategy> primitives{};
    };

    class Mesh {
    public:
        template<Core::AllocationStrategy strategy>
        explicit Mesh(MeshData<strategy>& meshData) {

        }

    private:
        bgfx::VertexLayout vertexLayout{};
        bgfx::VertexBufferHandle vbo{};
        bgfx::IndexBufferHandle ibo{};
        Vector<Primitive> primitives{};
    };

    template<>
    struct ResourceLoader<Mesh> {
        Ref<Mesh> operator()(const Ref<IO::File>& file) {
            return Vin::MakeRef<Mesh>();
        }
    };
}

#endif //VIN_ENGINE_MESH_H
