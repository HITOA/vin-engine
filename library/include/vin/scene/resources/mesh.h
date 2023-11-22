#ifndef VIN_ENGINE_MESH_H
#define VIN_ENGINE_MESH_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <bgfx/bgfx.h>
#include <vin/scene/resources/material.h>

namespace Vin {

    struct Primitive {
        uint32_t startVertex;
        uint32_t numVertices;
        uint32_t startIndex;
        uint32_t numIndices;
        Ref<Material> material{};
    };


    template<typename TVertex, typename TIndex = uint16_t, Core::AllocationStrategy strategy = Core::AllocationStrategy::Persistent>
    struct MeshData {
        using IndexType = TIndex;

        bgfx::VertexLayout vertexLayout{};
        Vector<TVertex, strategy> vertices{};
        Vector<IndexType, strategy> indices{};
        Vector<Primitive, strategy> primitives{};
    };

    //Default Static Mesh
    class Mesh {
    public:
        template<typename TVertex, typename TIndex, Core::AllocationStrategy strategy>
        explicit Mesh(const MeshData<TVertex, TIndex, strategy>& meshData) :
            vertexLayout{ meshData.vertexLayout }, primitives{ meshData.primitives.data(), meshData.primitives.data() + meshData.primitives.size() } {
            const bgfx::Memory* verticesRef = bgfx::makeRef(meshData.vertices.data(), meshData.vertices.size() * sizeof(TVertex));
            const bgfx::Memory* indicesRef = bgfx::makeRef(meshData.indices.data(), meshData.indices.size() * sizeof(TIndex));
            vbo = bgfx::createVertexBuffer(verticesRef, vertexLayout);
            if constexpr (sizeof(TIndex) == sizeof(uint32_t)) {
                ibo = bgfx::createIndexBuffer(indicesRef, BGFX_BUFFER_INDEX32);
            } else {
                ibo = bgfx::createIndexBuffer(indicesRef, BGFX_BUFFER_NONE);
            }
        }

        ~Mesh();

        const bgfx::VertexLayout& GetVertexLayout();
        bgfx::VertexBufferHandle GetVertexBufferHandle();
        bgfx::IndexBufferHandle GetIndexBufferHandle();

        size_t GetPrimitivesCount();
        const Primitive& GetPrimitive(size_t idx);

        [[nodiscard]] Vector<Primitive>::const_iterator begin() const;
        [[nodiscard]] Vector<Primitive>::const_iterator end() const;
        [[nodiscard]] Vector<Primitive>::const_iterator cbegin() const;
        [[nodiscard]] Vector<Primitive>::const_iterator cend() const;

    private:
        bgfx::VertexLayout vertexLayout{};
        bgfx::VertexBufferHandle vbo{};
        bgfx::IndexBufferHandle ibo{};
        Vector<Primitive> primitives{};
    };

    /*template<>
    struct ResourceLoader<Mesh> {
        Ref<Mesh> operator()(const Ref<IO::File>& file) {
            //return Vin::MakeRef<Mesh>();
        }
    };*/
}

#endif //VIN_ENGINE_MESH_H
