#ifndef VIN_EDITOR_SHADERIMPORTER_H
#define VIN_EDITOR_SHADERIMPORTER_H

#include "assetimporter.h"
#include <vin/asset/assetshader.h>
#include <vin/asset/aggregateshader.h>

class ShaderImporter : public AssetImporter {
public:
    struct ImportSettings : Vin::Serializable {
        Vin::ShaderType type{ Vin::ShaderType::Compute };

        [[nodiscard]] nlohmann::json Serialize() const final;
        void Deserialize(const nlohmann::json& data) final;
    };

public:
    [[nodiscard]] Vin::AssetType GetType() const final;
    //Check if file match the importer
    [[nodiscard]] bool Match(Vin::StringView path) const final;
    //Import asset from file.
    AssetRegistryID ImportFromFile(Vin::StringView path, const EditorImportSettings& editorImportSettings) final;
    //Import asset from memory. This will produce orphan asset and will not write any asset file onto the disk.
    AssetRegistryID ImportFromMemory(char* data, size_t size, Vin::Vector<char>& out, const EditorImportSettings& editorImportSettings) final;
    bool ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings& editorImportSettings) final;

private:
    size_t CompileShader(Vin::StringView path, const EditorImportSettings& editorImportSettings, const ImportSettings& shaderImportSettings, Vin::Vector<char>& out);
    void ProcessShaderCode(Vin::Vector<char>& out, nlohmann::json& reflection, const char* source, size_t size, Vin::ShaderType type, bool glslRenameAttribute = false);

    Vin::ShaderType GetShaderStage(Vin::StringView path);
};

#endif
