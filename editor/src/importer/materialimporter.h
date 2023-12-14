#ifndef VIN_EDITOR_MATERIALIMPORTER_H
#define VIN_EDITOR_MATERIALIMPORTER_H

#include "assetimporter.h"

class MaterialImporter : public AssetImporter {
public:
    [[nodiscard]] Vin::AssetType GetType() const final;
    //Check if file match the importer
    [[nodiscard]] bool Match(Vin::StringView path) const final;
    //Import asset from file.
    AssetRegistryID ImportFromFile(Vin::StringView path, const EditorImportSettings& editorImportSettings) final;
    //Import asset from memory. This will produce orphan asset.
    AssetRegistryID ImportFromMemory(char* data, size_t size, Vin::Vector<char>& out, const EditorImportSettings& editorImportSettings) final;
    bool ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings& editorImportSettings) final;
};

#endif
