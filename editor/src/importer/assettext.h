#ifndef VIN_EDITOR_ASSETTEXT_H
#define VIN_EDITOR_ASSETTEXT_H

#include "assetimporter.h"
#include <filesystem>
#include <fstream>

/*
struct AssetTextImportSettings {
    bool compressed{ false };
};

template<>
struct AssetImporter<Vin::AssetType::Text> {
    Vin::String operator()(Vin::StringView path, EditorImportSettings& editorImportSettings, AssetTextImportSettings& textImportSettings) {
        std::filesystem::path filePath{ path };
        std::filesystem::path assetPath{ path };
        assetPath.replace_extension(".vasset");

        std::fstream file{ filePath, std::fstream::in | std::fstream::binary };
        std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

        file.ignore( std::numeric_limits<std::streamsize>::max() );
        size_t size = file.gcount();
        file.clear();
        file.seekg(0, std::ios_base::beg);

        Vin::AssetFileHeader assetFileHeader{};
        memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
        assetFileHeader.type = Vin::AssetType::Text;
        assetFileHeader.size = size;

        asset.write((const char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));

        std::vector<char> fileContent{};
        fileContent.resize(size);
        file.read(fileContent.data(), fileContent.size());

        if (textImportSettings.compressed)
            Vin::Logger::Warn("Text compression not yet implemented.");

        asset.write(fileContent.data(), fileContent.size());

        file.close();
        asset.close();

        return PATH_TO_STRING(assetPath);
    }
};*/

#endif //VIN_EDITOR_ASSETTEXT_H
