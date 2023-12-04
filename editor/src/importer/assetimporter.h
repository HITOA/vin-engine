#ifndef VIN_EDITOR_ASSETIMPORTER_H
#define VIN_EDITOR_ASSETIMPORTER_H

#include <vin/vin.h>
#include "../project/project.h"
#include "../config/editorimportsettings.h"

class AssetImporter {
public:
    virtual ~AssetImporter() = default;

    [[nodiscard]] virtual Vin::AssetType GetType() const = 0;
    //Check if file match the importer
    [[nodiscard]] virtual bool Match(Vin::StringView path) const = 0;
    //Import asset from file.
    virtual AssetRegistryID ImportFromFile(Vin::StringView path, const EditorImportSettings& editorImportSettings) = 0;
    //Import asset from memory. This will produce orphan asset.
    virtual AssetRegistryID ImportFromMemory(char* data, size_t size, Vin::Vector<char>& out, const EditorImportSettings& editorImportSettings) = 0;
    virtual bool ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings& editorImportSettings) = 0;

public:
    Vin::Ref<Project> project{};
};

#endif //VIN_EDITOR_ASSETIMPORTER_H
