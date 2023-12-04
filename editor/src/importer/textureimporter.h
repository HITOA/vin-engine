#ifndef VIN_EDITOR_TEXTUREIMPORTER_H
#define VIN_EDITOR_TEXTUREIMPORTER_H

#include "assetimporter.h"


enum class TextureEncodingFormat {
    BC1, //RGB + 1bit Alpha
    BC3, //BC1 RGB + BC4 Alpha
    BC4, //Grayscale
    BC5  //2xBC4 RG
};

enum class TextureEncodingQuality {
    Default,
    Fastest,
    Highest
};

class TextureImporter : public AssetImporter {
public:
    struct ImportSettings : Vin::Serializable {
        TextureEncodingFormat encodingFormat{ TextureEncodingFormat::BC1 };
        TextureEncodingQuality encodingQuality{ TextureEncodingQuality::Default };
        bool mips{ true }; //Generate Mipmaps
        bool linear{ false }; //No gamma correction
        bool pma{ false }; //Premultiply alpha into rgb channel

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
    void SetImportSettings(const ImportSettings& textureImportSettings);
    ImportSettings GetImportSettings();

private:
    size_t CompileTexture(Vin::StringView path, const EditorImportSettings& editorImportSettings, const ImportSettings& textureImportSettings, Vin::Vector<char>& out);

private:
    ImportSettings textureImportSettings{};
};

#endif //VIN_EDITOR_TEXTUREIMPORTER_H
