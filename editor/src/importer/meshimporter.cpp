#include "meshimporter.h"
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "textureimporter.h"
#include <vin/core/templates/scratch.h>
#include <bgfx/bgfx.h>
#include <limits>

nlohmann::json MeshImporter::ImportSettings::Serialize() const {
    nlohmann::json serializedData{};

    serializedData["Optimize Mesh"] = optimizeMesh;
    serializedData["Generate UV"] = generateUV;
    serializedData["Include Tangent"] = includeTangent;
    serializedData["Include Bitangent"] = includeBitangent;

    return serializedData;
}

void MeshImporter::ImportSettings::Deserialize(const nlohmann::json &data) {
    if (data.count("Optimize Mesh"))
        optimizeMesh = data["Optimize Mesh"].get<bool>();
    if (data.count("Generate UV"))
        generateUV = data["Generate UV"].get<bool>();
    if (data.count("Include Tangent"))
        includeTangent = data["Include Tangent"].get<bool>();
    if (data.count("Include Bitangent"))
        includeBitangent = data["Include Bitangent"].get<bool>();
}

Vin::AssetType MeshImporter::GetType() const {
    return Vin::AssetType::Mesh;
}

bool MeshImporter::Match(Vin::StringView path) const {
    Assimp::Importer importer{};

    const aiScene* scene = importer.ReadFile(path.data(), 0);
    return scene;
}

/**
 * Mesh can be imported only if all submesh can share the same vertex layout
 */
AssetRegistryID MeshImporter::ImportFromFile(Vin::StringView path, const EditorImportSettings &editorImportSettings) {
    std::filesystem::path assetPath{ path };
    assetPath.replace_extension(".vasset");

    Assimp::Importer importer{};

    unsigned int importerFlags = aiProcess_Triangulate |
                                 aiProcess_SortByPType |
                                 aiProcess_GenNormals |
                                 aiProcess_CalcTangentSpace |
                                 aiProcess_RemoveRedundantMaterials |
                                 aiProcess_PreTransformVertices |
                                 aiProcess_GenBoundingBoxes; /*|
                                 aiProcess_EmbedTextures; |
                                 aiProcess_SplitLargeMeshes;*/

    if (meshImportSettings.optimizeMesh)
        importerFlags |= aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality | aiProcess_OptimizeMeshes;
    if (meshImportSettings.generateUV)
        importerFlags |= aiProcess_GenUVCoords;

    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, AI_SLM_DEFAULT_MAX_VERTICES);
    importer.SetPropertyInteger(AI_CONFIG_PP_SLM_TRIANGLE_LIMIT, AI_SLM_DEFAULT_MAX_TRIANGLES);

    const aiScene* scene = importer.ReadFile(path.data(), importerFlags);

    if (!scene) {
        Vin::Logger::Err("Error while importing mesh : ", path);
        return "";
    }

    if (scene->mNumLights)
        Vin::Logger::Warn("The provided file contain light(s). These will not be added unless you import it as a scene.");

    if (scene->mNumCameras)
        Vin::Logger::Warn("The provided file contain camera(s). These will not be added unless you import it as a scene.");

    uint32_t maxVerticesCount{ 0 };
    bool isIndices32{ false };
    bgfx::VertexLayout vertexLayout{};

    vertexLayout.begin();

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];

        if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE))
            continue;

        if (maxVerticesCount < mesh->mNumVertices)
            maxVerticesCount = mesh->mNumVertices;

        vertexLayout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);

        if (mesh->HasNormals())
            vertexLayout.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float, true);

        if (mesh->HasTangentsAndBitangents()) {
            if (meshImportSettings.includeTangent)
                vertexLayout.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float, true);
            if (meshImportSettings.includeBitangent)
                vertexLayout.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float, true);
        }

        for (uint32_t j = 0; j < 4; ++j)
            if (mesh->HasVertexColors(j))
                vertexLayout.add((bgfx::Attrib::Enum)((int)bgfx::Attrib::Color0 + j), 4, bgfx::AttribType::Uint8);

        //Will unroll
        for (uint32_t j = 0; j < 8; ++j) {
            if (mesh->HasTextureCoords(j)) {
                if (vertexLayout.has((bgfx::Attrib::Enum)((int)bgfx::Attrib::TexCoord0 + j))) {
                    uint16_t val = vertexLayout.m_attributes[(int)bgfx::Attrib::TexCoord0 + j];
                    uint16_t num = (val&3)+1;
                    if (num < mesh->mNumUVComponents[j]) {
                        vertexLayout.add((bgfx::Attrib::Enum) ((int) bgfx::Attrib::TexCoord0 + j), mesh->mNumUVComponents[j],
                                         bgfx::AttribType::Float);
                    }
                } else {
                    vertexLayout.add((bgfx::Attrib::Enum) ((int) bgfx::Attrib::TexCoord0 + j), mesh->mNumUVComponents[j],
                                     bgfx::AttribType::Float);
                }
            }
        }
    }

    isIndices32 = maxVerticesCount >= std::numeric_limits<uint16_t>::max();

    vertexLayout.end();

    Vin::ScratchBuffer<> meshData{};
    meshData.Write((uint8_t *) vertexLayout.m_attributes, bgfx::Attrib::Count * sizeof(uint16_t));

    {
        Vin::ScratchBuffer<> vertexBuffer{};
        Vin::ScratchBuffer<> indexBuffer{};
        Vin::Vector<PrimitiveInfo> primitives{};

        uint32_t vertexOffset{0};
        uint32_t indexOffset{0};

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            aiMesh *mesh = scene->mMeshes[i];

            if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE))
                continue;

            PrimitiveInfo primitiveInfo{};
            primitiveInfo.startVertex = vertexOffset;
            primitiveInfo.startIndex = indexOffset;
            primitiveInfo.numVertices = mesh->mNumVertices;
            primitiveInfo.numIndices = mesh->mNumFaces * 3;
            vertexOffset += primitiveInfo.numVertices;
            indexOffset += primitiveInfo.numIndices;
            primitives.push_back(primitiveInfo);

            BuildVertexBuffer(vertexBuffer, mesh, vertexLayout);
            if (isIndices32)
                BuildIndex32Buffer(indexBuffer, mesh, 0);
            else
                BuildIndex16Buffer(indexBuffer, mesh, 0);
        }

        meshData.Write<uint32_t>(vertexBuffer.GetSize());
        meshData.Write(vertexBuffer.GetData(), vertexBuffer.GetSize());
        meshData.Write<uint8_t>(isIndices32);
        meshData.Write<uint32_t>(indexBuffer.GetSize());
        meshData.Write(indexBuffer.GetData(), indexBuffer.GetSize());

        meshData.Write<uint8_t>(primitives.size());
        for (auto& primitive : primitives) {
            meshData.Write(primitive.startVertex);
            meshData.Write(primitive.numVertices);
            meshData.Write(primitive.startIndex);
            meshData.Write(primitive.numIndices);
        }

    }

    std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

    Vin::AssetFileHeader assetFileHeader{};
    memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
    assetFileHeader.type = Vin::AssetType::Mesh;
    assetFileHeader.depCount = 0;
    assetFileHeader.size = meshData.GetSize();

    asset.write((char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));
    asset.write((char*)meshData.GetData(), meshData.GetSize());
    asset.close();

    AssetRegistryEntry assetEntry{};
    assetEntry.type = Vin::AssetType::Mesh;
    assetEntry.metadata["Import Settings"] = meshImportSettings.Serialize();
    assetEntry.metadata["Imported Asset Path"] = PATH_TO_STRING(std::filesystem::relative(assetPath, project->GetWorkingDirectory()));

    AssetRegistryID assetId = PATH_TO_STRING(std::filesystem::relative(path, project->GetWorkingDirectory()));
    project->GetAssetRegistry()->AddAsset(assetId, assetEntry);

    return assetId;

    /*for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];

        Vin::ScratchBuffer vertexBuffer{};
        Vin::ScratchBuffer indexBuffer{};
        bool isIndex32 = mesh->mNumVertices >= std::numeric_limits<uint16_t>::max();
        BuildVertexBuffer(vertexBuffer, mesh);

        if (isIndex32)
            BuildIndex32Buffer(indexBuffer, mesh);
        else
            BuildIndex16Buffer(indexBuffer, mesh);


    }*/

    /*TextureImporter textureImporter{};
    textureImporter.project = project;

    Vin::Vector<char> compiledTexture{};

    for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
        aiTexture* texture = scene->mTextures[i];
        AssetRegistryID id{};
        if (texture->mHeight == 0)
            id = textureImporter.ImportFromMemory((char*)texture->pcData, texture->mWidth, compiledTexture, editorImportSettings);
        else
            id = textureImporter.ImportFromMemory((char*)texture->pcData, texture->mWidth * texture->mHeight, compiledTexture, editorImportSettings);
        entry.children.push_back(id);
        //Do Shit With Compiled Texture
    }

    return "";*/
}

AssetRegistryID MeshImporter::ImportFromMemory(char *data, size_t size, Vin::Vector<char> &out,
                                               const EditorImportSettings &editorImportSettings) {

}

bool MeshImporter::ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings &editorImportSettings) {

}

void MeshImporter::BuildVertexBuffer(Vin::ScratchBuffer<>& buffer, aiMesh* mesh, bgfx::VertexLayout& layout) {
    for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
        buffer.Write(mesh->mVertices[i]);

        if (mesh->HasNormals() && layout.has(bgfx::Attrib::Normal))
            buffer.Write(mesh->mNormals[i]);

        if (mesh->HasTangentsAndBitangents()) {
            if (layout.has(bgfx::Attrib::Tangent))
                buffer.Write(mesh->mTangents[i]);
            if (layout.has(bgfx::Attrib::Bitangent))
                buffer.Write(mesh->mBitangents[i]);
        } else {
            if (layout.has(bgfx::Attrib::Tangent))
                buffer.Write<aiVector3D>({ 0.0f, 0.0f, 0.0f });
            if (layout.has(bgfx::Attrib::Bitangent))
                buffer.Write<aiVector3D>({ 0.0f, 0.0f, 0.0f });
        }

        //Will unroll
        for (uint32_t j = 0; j < 4; ++j) {
            if (mesh->HasVertexColors(j) && layout.has((bgfx::Attrib::Enum)((int)bgfx::Attrib::Color0 + j))) {
                for (uint32_t k = 0; k < 4; ++k)
                    buffer.Write<uint8_t>(mesh->mColors[j][i][k]);
            } else if (layout.has((bgfx::Attrib::Enum)((int)bgfx::Attrib::Color0 + j))) {
                buffer.Write<uint32_t>(0);
            }
        }

        //Will unroll
        for (uint32_t j = 0; j < 8; ++j) {
            if (mesh->HasTextureCoords(j) && layout.has((bgfx::Attrib::Enum)((int)bgfx::Attrib::TexCoord0 + j))) {
                uint16_t val = layout.m_attributes[(int)bgfx::Attrib::TexCoord0 + j];
                uint16_t num = (val&3)+1;
                for (uint8_t k = 0; k < num; ++k) {
                    if (k >= mesh->mNumUVComponents[j])
                        buffer.Write<ai_real>(0.0f);
                    else
                        buffer.Write(mesh->mTextureCoords[j][i][k]);
                }
            }
        }
    }
}

void MeshImporter::BuildIndex32Buffer(Vin::ScratchBuffer<> &buffer, aiMesh *mesh, uint32_t offset) {
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
        for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
            buffer.Write<uint32_t>(mesh->mFaces[i].mIndices[j] + offset);
        }
    }
}

void MeshImporter::BuildIndex16Buffer(Vin::ScratchBuffer<> &buffer, aiMesh *mesh, uint16_t offset) {
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
        for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
            buffer.Write<uint16_t>(mesh->mFaces[i].mIndices[j] + offset);
        }
    }
}