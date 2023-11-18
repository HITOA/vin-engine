#include "project.h"
#include <fstream>
#include <filesystem>
#include <vin/json/json.hpp>
#include <vin/core/templates/hash.h>

Project::Project(Vin::StringView path) {
    projectFilePath = path;
    if (std::filesystem::exists(projectFilePath)) {
        LoadProjectFile();
    } else {
        SaveProjectFile();
    }
}

Project::~Project() {
    SaveProjectFile();
}

void Project::LoadProjectFile() {
    std::fstream projectFile{ std::filesystem::path{ projectFilePath }, std::fstream::in | std::fstream::binary };
    nlohmann::json jsonProject = nlohmann::json::parse(projectFile);
    projectFile.close();

    if (jsonProject.contains("name"))
        name = jsonProject["name"].get<Vin::String>();

    if (jsonProject.contains("importedTextAsset")) {
        for (auto& entry : jsonProject["importedTextAsset"]) {
            ImportedAssetEntry<AssetTextImportSettings> curr{};
            curr.originalAssetFilePath = entry["originalAssetFilePath"].get<std::string>();
            curr.importedAssetFilePath = entry["importedAssetFilePath"].get<std::string>();
            curr.importSettings.compressed = entry["importSettings"]["compressed"].get<bool>();
            importedTextAsset.push_back(curr);
        }
    }

    if (jsonProject.contains("importedTextureAsset")) {
        for (auto& entry : jsonProject["importedTextureAsset"]) {
            ImportedAssetEntry<AssetTextureImportSettings> curr{};
            curr.originalAssetFilePath = entry["originalAssetFilePath"].get<std::string>();
            curr.importedAssetFilePath = entry["importedAssetFilePath"].get<std::string>();
            curr.importSettings.encodingFormat = (TextureEncodingFormat)entry["importSettings"]["encodingFormat"].get<int>();
            curr.importSettings.encodingQuality = (TextureEncodingQuality)entry["importSettings"]["encodingQuality"].get<int>();
            curr.importSettings.mips = entry["importSettings"]["mipmaps"].get<bool>();
            curr.importSettings.linear = entry["importSettings"]["linear"].get<bool>();
            curr.importSettings.pma = entry["importSettings"]["pma"].get<bool>();
            importedTextureAsset.push_back(curr);
        }
    }

    if (jsonProject.contains("importedShaderAsset")) {
        for (auto& entry : jsonProject["importedShaderAsset"]) {
            ImportedAssetEntry<AssetShaderImportSettings> curr{};
            curr.originalAssetFilePath = entry["originalAssetFilePath"].get<std::string>();
            curr.importedAssetFilePath = entry["importedAssetFilePath"].get<std::string>();
            curr.importSettings.type = (Vin::ShaderType)entry["importSettings"]["shaderType"].get<int>();
            importedShaderAsset.push_back(curr);
        }
    }
}

void Project::SaveProjectFile() {
    nlohmann::json jsonProject{};
    jsonProject["name"] = name;

    nlohmann::json jsonImportedTextAssets{};
    for (auto& entry : importedTextAsset) {
        nlohmann::json curr{};
        curr["originalAssetFilePath"] = entry.originalAssetFilePath;
        curr["importedAssetFilePath"] = entry.importedAssetFilePath;
        curr["importSettings"]["compressed"] = entry.importSettings.compressed;

        jsonImportedTextAssets += curr;
    }

    nlohmann::json jsonImportedTextureAssets{};
    for (auto& entry : importedTextureAsset) {
        nlohmann::json curr{};
        curr["originalAssetFilePath"] = entry.originalAssetFilePath;
        curr["importedAssetFilePath"] = entry.importedAssetFilePath;
        curr["importSettings"]["encodingFormat"] = entry.importSettings.encodingFormat;
        curr["importSettings"]["encodingQuality"] = entry.importSettings.encodingQuality;
        curr["importSettings"]["mipmaps"] = entry.importSettings.mips;
        curr["importSettings"]["linear"] = entry.importSettings.linear;
        curr["importSettings"]["pma"] = entry.importSettings.pma;

        jsonImportedTextureAssets += curr;
    }

    nlohmann::json jsonImportedShaderAssets{};
    for (auto& entry : importedShaderAsset) {
        nlohmann::json curr{};
        curr["originalAssetFilePath"] = entry.originalAssetFilePath;
        curr["importedAssetFilePath"] = entry.importedAssetFilePath;
        curr["importSettings"]["shaderType"] = entry.importSettings.type;

        jsonImportedShaderAssets += curr;
    }

    jsonProject["importedTextAsset"] = jsonImportedTextAssets;
    jsonProject["importedTextureAsset"] = jsonImportedTextureAssets;
    jsonProject["importedShaderAsset"] = jsonImportedShaderAssets;
    std::fstream projectFile{ std::filesystem::path{ projectFilePath }, std::fstream::out | std::fstream::binary | std::fstream::trunc };
    projectFile << jsonProject.dump(4);
}

void Project::ImportTextAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath,
                              AssetTextImportSettings &textImportSettings) {
    auto it = std::find_if(importedTextAsset.begin(), importedTextAsset.end(), [&](ImportedAssetEntry<AssetTextImportSettings>& entry){
        return entry.originalAssetFilePath == originalAssetPath;
    });
    if (it != importedTextAsset.end()) {
        it->importedAssetFilePath = importedAssetPath;
        it->importSettings = textImportSettings;
    } else {
        ImportedAssetEntry<AssetTextImportSettings> entry{};
        entry.originalAssetFilePath = originalAssetPath;
        entry.importedAssetFilePath = importedAssetPath;
        entry.importSettings = textImportSettings;
        importedTextAsset.push_back(entry);
    }
    SaveProjectFile();
}

AssetTextImportSettings Project::GetTextAssetImportSettings(Vin::StringView path) {
    auto it = std::find_if(importedTextAsset.begin(), importedTextAsset.end(), [&](ImportedAssetEntry<AssetTextImportSettings>& entry){
        return entry.originalAssetFilePath == path;
    });
    if (it != importedTextAsset.end())
        return it->importSettings;
    return AssetTextImportSettings{};
}

void Project::ImportTextureAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath,
                                 AssetTextureImportSettings &textureImportSettings) {
    auto it = std::find_if(importedTextureAsset.begin(), importedTextureAsset.end(), [&](ImportedAssetEntry<AssetTextureImportSettings>& entry){
        return entry.originalAssetFilePath == originalAssetPath;
    });
    if (it != importedTextureAsset.end()) {
        it->importedAssetFilePath = importedAssetPath;
        it->importSettings = textureImportSettings;
    } else {
        ImportedAssetEntry<AssetTextureImportSettings> entry{};
        entry.originalAssetFilePath = originalAssetPath;
        entry.importedAssetFilePath = importedAssetPath;
        entry.importSettings = textureImportSettings;
        importedTextureAsset.push_back(entry);
    }
    SaveProjectFile();
}

AssetTextureImportSettings Project::GetTextureAssetImportSettings(Vin::StringView path) {
    auto it = std::find_if(importedTextureAsset.begin(), importedTextureAsset.end(), [&](ImportedAssetEntry<AssetTextureImportSettings>& entry){
        return entry.originalAssetFilePath == path;
    });
    if (it != importedTextureAsset.end())
        return it->importSettings;
    return AssetTextureImportSettings{};
}

void Project::ImportShaderAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath,
                                AssetShaderImportSettings &shaderImportSettings) {
    auto it = std::find_if(importedShaderAsset.begin(), importedShaderAsset.end(), [&](ImportedAssetEntry<AssetShaderImportSettings>& entry){
        return entry.originalAssetFilePath == originalAssetPath;
    });
    if (it != importedShaderAsset.end()) {
        it->importedAssetFilePath = importedAssetPath;
        it->importSettings = shaderImportSettings;
    } else {
        ImportedAssetEntry<AssetShaderImportSettings> entry{};
        entry.originalAssetFilePath = originalAssetPath;
        entry.importedAssetFilePath = importedAssetPath;
        entry.importSettings = shaderImportSettings;
        importedShaderAsset.push_back(entry);
    }
    SaveProjectFile();
}

AssetShaderImportSettings Project::GetShaderAssetImportSettings(Vin::StringView path) {
    auto it = std::find_if(importedShaderAsset.begin(), importedShaderAsset.end(), [&](ImportedAssetEntry<AssetShaderImportSettings>& entry){
        return entry.originalAssetFilePath == path;
    });
    if (it != importedShaderAsset.end())
        return it->importSettings;
    return AssetShaderImportSettings{};
}

bool Project::IsAssetImported(Vin::StringView path) {
    if (std::find_if(importedTextAsset.begin(), importedTextAsset.end(), [&](ImportedAssetEntry<AssetTextImportSettings>& entry){
        return entry.originalAssetFilePath == path;
    }) != importedTextAsset.end()) {
        return true;
    }

    if (std::find_if(importedTextureAsset.begin(), importedTextureAsset.end(), [&](ImportedAssetEntry<AssetTextureImportSettings>& entry){
        return entry.originalAssetFilePath == path;
    }) != importedTextureAsset.end()) {
        return true;
    }

    if (std::find_if(importedShaderAsset.begin(), importedShaderAsset.end(), [&](ImportedAssetEntry<AssetShaderImportSettings>& entry){
        return entry.originalAssetFilePath == path;
    }) != importedShaderAsset.end()) {
        return true;
    }

    return false;
}

template<typename T>
inline void RemoveEntryIfExist(Vin::StringView path, T& entries, std::filesystem::path& workingDir) {
    auto it = std::find_if(entries.begin(), entries.end(), [&](auto& entry){
        return entry.originalAssetFilePath == path;
    });
    if (it != entries.end()) {
        std::filesystem::path fullPath{ workingDir / it->importedAssetFilePath };
        std::remove(PATH_TO_STRING(fullPath).c_str());
        entries.erase(it);
    }
}

void Project::RemoveAssetFromProject(Vin::StringView path) {
    std::filesystem::path workingDir{ projectFilePath };
    workingDir = workingDir.remove_filename();

    RemoveEntryIfExist(path, importedTextAsset, workingDir);
    RemoveEntryIfExist(path, importedTextureAsset, workingDir);
    RemoveEntryIfExist(path, importedShaderAsset, workingDir);
    SaveProjectFile();
}