#ifndef VIN_EDITOR_MESHIMPORTER_H
#define VIN_EDITOR_MESHIMPORTER_H

#include "assetimporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vin/core/templates/scratch.h>
#include <bgfx/bgfx.h>

class MeshImporter : public AssetImporter {
public:
    struct ImportSettings : Vin::Serializable {
        bool optimizeMesh{ true };
        bool generateUV{ true };
        bool includeTangent{ true };
        bool includeBitangent{ false };

        [[nodiscard]] nlohmann::json Serialize() const final;
        void Deserialize(const nlohmann::json& data) final;
    };

public:
    [[nodiscard]] Vin::AssetType GetType() const final;
    //Check if file match the importer
    [[nodiscard]] bool Match(Vin::StringView path) const final;
    //Import asset from file.
    AssetRegistryID ImportFromFile(Vin::StringView path, const EditorImportSettings& editorImportSettings) final;
    //Import asset from memory. This will produce orphan asset.
    AssetRegistryID ImportFromMemory(char* data, size_t size, Vin::Vector<char>& out, const EditorImportSettings& editorImportSettings) final;
    bool ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings& editorImportSettings) final;

private:
    void BuildVertexBuffer(Vin::ScratchBuffer<>& buffer, aiMesh* mesh, bgfx::VertexLayout& layout);
    void BuildIndex32Buffer(Vin::ScratchBuffer<>& buffer, aiMesh* mesh, uint32_t offset);
    void BuildIndex16Buffer(Vin::ScratchBuffer<>& buffer, aiMesh* mesh, uint16_t offset);

    struct PrimitiveInfo {
        uint32_t startVertex;
        uint32_t numVertices;
        uint32_t startIndex;
        uint32_t numIndices;
        //Material ID
    };

private:
    ImportSettings meshImportSettings{};

};

#endif
