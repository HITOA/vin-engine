#ifndef VIN_EDITOR_ASSETMESH_H
#define VIN_EDITOR_ASSETMESH_H

#include "assetimporter.h"
#include <filesystem>
#include <fstream>
/*#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>*/
/*
struct AssetMeshImportSettings {
};

template<>
struct AssetImporter<Vin::AssetType::Text> {
    Vin::String operator()(Vin::StringView path, EditorImportSettings& editorImportSettings, AssetMeshImportSettings& meshImportSettings) {
        std::filesystem::path assetPath{ path };
        assetPath.replace_extension(".vasset");

        std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

        Assimp::Importer importer{};

        unsigned int importerFlags = aiProcess_Triangulate |
                aiProcess_JoinIdenticalVertices |
                aiProcess_SortByPType |
                aiProcess_CalcTangentSpace |
                aiProcess_GenUVCoords;

        const aiScene* scene = importer.ReadFile(path.data(), importerFlags);

        if (!scene) {
            Vin::Logger::Err("Error while importing mesh : ", path);
            return "";
        }




        return PATH_TO_STRING(assetPath);
    }
};
*/
#endif
