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

}

void Project::SaveProjectFile() {
    nlohmann::json jsonProject{};
    jsonProject["name"] = name;

    nlohmann::json jsonImportedAssets{ nlohmann::json::array() };
    for (auto& entry : importedAssets) {
        jsonImportedAssets += { entry.first, (int)entry.second };
    }

    jsonProject["importedAsset"] = jsonImportedAssets;
    std::fstream projectFile{ std::filesystem::path{ projectFilePath }, std::fstream::out | std::fstream::trunc };
    projectFile << jsonProject.dump(4);
}

void Project::ImportAsset(Vin::StringView path, Vin::AssetType type) {
    importedAssets[Vin::String{ path }] = type;
    SaveProjectFile();
}

void Project::UnimportAsset(Vin::StringView path) {

}