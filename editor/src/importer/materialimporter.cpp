#include "materialimporter.h"

Vin::AssetType MaterialImporter::GetType() const {
    return Vin::AssetType::Material;
}

bool MaterialImporter::Match(Vin::StringView path) const {
    return false;
}

AssetRegistryID MaterialImporter::ImportFromFile(Vin::StringView path, const EditorImportSettings &editorImportSettings) {
    return "";
}

AssetRegistryID MaterialImporter::ImportFromMemory(char *data, size_t size, Vin::Vector<char> &out,
                                                   const EditorImportSettings &editorImportSettings) {
    return "";
}

bool MaterialImporter::ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings &editorImportSettings) {
    return false;
}