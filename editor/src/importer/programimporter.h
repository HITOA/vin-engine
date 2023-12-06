#ifndef VIN_EDITOR_PROGRAMIMPORTER_H
#define VIN_EDITOR_PROGRAMIMPORTER_H

#include "assetimporter.h"
#include "shaderimporter.h"

class ProgramImporter : public AssetImporter {
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
    enum class VinShaderToken {
        Identifier,
        String,
        Number,
        Colon,
        Equal,
        Coma,
        LPar,
        RPar,
        BlkStart,
        BlkEnd,
        Vertex,
        Fragment
    };

    struct Token {
        Vin::StringView token;
        VinShaderToken type;

        Token(Vin::StringView token, VinShaderToken type) : token{ token }, type{ type } {};
    };

    Vin::Vector<Token> Tokenize(const char* data, size_t size);
    int ParseField(const Vin::Vector<Token>& tokens, int startIdx, nlohmann::json& programMetadata);
};

#endif
