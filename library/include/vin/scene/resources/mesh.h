#ifndef VIN_ENGINE_MESH_H
#define VIN_ENGINE_MESH_H

#include <vin/core/templates/stdcontainers.h>
#include <vin/resource/resourceloader.h>
#include <vin/scene/resources/material.h>

namespace Vin {

    /*struct Primitive {
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

        Mesh(bgfx::VertexLayout& layout, const bgfx::Memory* vertexBuffer, const bgfx::Memory* indexBuffer,
             Vin::Vector<Primitive>& primitives, bool isIndices32 = false) : vertexLayout{ layout }, primitives{ primitives } {
            vbo = bgfx::createVertexBuffer(vertexBuffer, vertexLayout);
            if (isIndices32) {
                ibo = bgfx::createIndexBuffer(indexBuffer, BGFX_BUFFER_INDEX32);
            } else {
                ibo = bgfx::createIndexBuffer(indexBuffer, BGFX_BUFFER_NONE);
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

    template<>
    struct ResourceLoader<Mesh> {
        Ref<Mesh> operator()(const Ref<IO::File>& file) {
            AssetFileHeader header{};
            if (file->ReadBytes((char*)&header, sizeof(AssetFileHeader)) != sizeof(AssetFileHeader) || !CheckMagic(header)) {
                Vin::Logger::Err("Couldn't load Mesh. Provided asset file isn't in the right format or is corrupted.");
                return Ref<Mesh>{};
            }

            if (header.type != Vin::AssetType::Mesh) {
                Vin::Logger::Err("Couldn't load Mesh. Provided asset isn't a Mesh.");
                return Ref<Mesh>{};
            }

            bgfx::VertexLayout vertexLayout{};
            const bgfx::Memory* vertexBuffer{};
            const bgfx::Memory* indexBuffer{};
            Vin::Vector<Primitive> primitives{};

            vertexLayout.begin();

            {
                bgfx::VertexLayout tmpLayout{};
                file->ReadBytes((char *) tmpLayout.m_attributes, bgfx::Attrib::Count * sizeof(uint16_t));

                for (int i = 0; i < bgfx::Attrib::Count; ++i) {
                    if (tmpLayout.has((bgfx::Attrib::Enum) i)) {
                        uint8_t num{};
                        bgfx::AttribType::Enum type{};
                        bool normalized{};
                        bool asInt{};
                        tmpLayout.decode((bgfx::Attrib::Enum)i, num, type, normalized, asInt);
                        vertexLayout.add((bgfx::Attrib::Enum)i, num, type, normalized, asInt);
                    }
                }
            }

            vertexLayout.end();

            uint32_t vertexBufferSize{};
            file->ReadBytes((char*)&vertexBufferSize, sizeof(uint32_t));
            vertexBuffer = bgfx::alloc(vertexBufferSize);
            file->ReadBytes((char*)vertexBuffer->data, vertexBuffer->size);

            uint8_t isIndices32{};
            uint32_t indexBufferSize{};

            file->ReadBytes((char*)&isIndices32, sizeof(uint8_t));
            file->ReadBytes((char*)&indexBufferSize, sizeof(uint32_t));
            indexBuffer = bgfx::alloc(indexBufferSize);
            file->ReadBytes((char*)indexBuffer->data, indexBuffer->size);

            uint8_t primitiveCount{};
            file->ReadBytes((char*)&primitiveCount, sizeof(uint8_t));

            for (int i = 0; i < primitiveCount; ++i) {
                Primitive primitive{};
                file->ReadBytes((char*)&primitive.startVertex, sizeof(uint32_t));
                file->ReadBytes((char*)&primitive.numVertices, sizeof(uint32_t));
                file->ReadBytes((char*)&primitive.startIndex, sizeof(uint32_t));
                file->ReadBytes((char*)&primitive.numIndices, sizeof(uint32_t));
                primitives.push_back(primitive);
            }

            return Vin::MakeRef<Mesh>(vertexLayout, vertexBuffer, indexBuffer, primitives, isIndices32);
        }
    };*/
}

#endif //VIN_ENGINE_MESH_H
