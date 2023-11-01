#ifndef VIN_ENGINE_PROJECT_H
#define VIN_ENGINE_PROJECT_H

#include <vin/vin.h>
#include "importer/assettext.h"
#include "importer/assettexture.h"

template<typename T>
struct ImportedAssetEntry {
    Vin::String originalAssetFilePath{};
    Vin::String importedAssetFilePath{};
    T importSettings{};
};

class Project {
public:
    Project() = delete;
    explicit Project(Vin::StringView path);
    ~Project();

    void LoadProjectFile();
    void SaveProjectFile();

    void ImportTextAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath, AssetTextImportSettings& textImportSettings);
    AssetTextImportSettings GetTextAssetImportSettings(Vin::StringView path);

    void ImportTextureAsset(Vin::StringView originalAssetPath, Vin::StringView importedAssetPath, AssetTextureImportSettings& textureImportSettings);
    AssetTextureImportSettings GetTextureAssetImportSettings(Vin::StringView path);

    bool IsAssetImported(Vin::StringView path);

private:


private:
    Vin::String name{ "New Project" };

    Vin::Vector<ImportedAssetEntry<AssetTextImportSettings>> importedTextAsset{};
    Vin::Vector<ImportedAssetEntry<AssetTextureImportSettings>> importedTextureAsset{};

    Vin::String projectFilePath{};
};

#endif //VIN_ENGINE_PROJECT_H
