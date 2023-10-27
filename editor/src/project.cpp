#include "project.h"
#include <filesystem>
#include <vin/json/json.hpp>

Project::Project(Vin::StringView path) {
    projectFilePath = path;
    if (std::filesystem::exists(projectFilePath)) {
        LoadProjectFile();
    } else {
        SaveProjectFile();
    }
}

Project::~Project() {
}

void Project::LoadProjectFile() {
    std::fstream projectFile{ std::filesystem::path{ projectFilePath }, std::fstream::in | std::fstream::binary };
    nlohmann::json jsonProject = nlohmann::json::parse(projectFile);
    projectFile.close();

    if (jsonProject.contains("name"))
        name = jsonProject["name"].get<Vin::String>();

    if (jsonProject.contains("importedAsset")) {
        for (auto& entry : jsonProject["importedAsset"]) {
            Vin::String assetPath = entry[0].get<Vin::String>();
            Vin::AssetType assetType = (Vin::AssetType)entry[1].get<int>();
            if (!importedAssets.count(assetPath))
                importedAssets[assetPath] = assetType;
        }
    }
}

void Project::SaveProjectFile() {
    nlohmann::json jsonProject{};
    jsonProject["name"] = name;

    nlohmann::json jsonImportedAssets{};
    for (auto& entry : importedAssets) {
        jsonImportedAssets += { entry.first, (int)entry.second };
    }

    jsonProject["importedAsset"] = jsonImportedAssets;
    std::fstream projectFile{ std::filesystem::path{ projectFilePath }, std::fstream::out | std::fstream::binary | std::fstream::trunc };
    projectFile << jsonProject.dump(4);
}

void Project::ImportAsset(Vin::StringView path, Vin::AssetType type) {
    importedAssets[Vin::String{ path }] = type;
    SaveProjectFile();
}

void Project::UnimportAsset(Vin::StringView path) {

}

bool Project::IsAssetImported(Vin::StringView path) {
    return importedAssets.count(Vin::String{ path });
}