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

    jsonProject["importedTextAsset"] = jsonImportedTextAssets;
    jsonProject["importedTextureAsset"] = jsonImportedTextureAssets;
    std::fstream projectFile{ std::filesystem::path{ projectFilePath }, std::fstream::out | std::fstream::binary | std::fstream::trunc };
    projectFile << jsonProject.dump(4);
}

void Project::ImportTextAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath,
                              AssetTextImportSettings &textImportSettings) {
    ImportedAssetEntry<AssetTextImportSettings> entry{};
    entry.originalAssetFilePath = originalAssetPath;
    entry.importedAssetFilePath = importedAssetPath;
    entry.importSettings = textImportSettings;
    importedTextAsset.push_back(entry);
    SaveProjectFile();
}

void Project::ImportTextureAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath,
                                 AssetTextureImportSettings &textureImportSettings) {
    ImportedAssetEntry<AssetTextureImportSettings> entry{};
    entry.originalAssetFilePath = originalAssetPath;
    entry.importedAssetFilePath = importedAssetPath;
    entry.importSettings = textureImportSettings;
    importedTextureAsset.push_back(entry);
    SaveProjectFile();
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

    return false;
}