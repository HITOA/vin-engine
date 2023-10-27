#ifndef VIN_ENGINE_ASSETIMPORTER_H
#define VIN_ENGINE_ASSETIMPORTER_H

#include <vin/vin.h>
#include <filesystem>
#include <fstream>
#include "config/editorimportsettings.h"

inline Vin::AssetType GetType(Vin::StringView ext) {
    static const char* textExt = ".txt";
    static const char* textureExt = ".bmp.dds.exr.gif.jpg.hdr.ktx.png.psd.pvr.tga";

    if (std::string{ textExt }.find(ext))
        return Vin::AssetType::Text;
    if (std::string{ textureExt }.find(ext))
        return Vin::AssetType::Texture;


    return Vin::AssetType::None;
}

template<Vin::AssetType type>
inline void GenerateAssetFile(Vin::StringView path, EditorImportSettings& importSettings) {
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
    assetFileHeader.type = type;
    assetFileHeader.size = size;

    asset.write((const char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));

    std::vector<char> fileContent{};
    fileContent.resize(size);
    file.read(fileContent.data(), fileContent.size());

    asset.write(fileContent.data(), fileContent.size());

    file.close();
    asset.close();
}

#endif //VIN_ENGINE_ASSETIMPORTER_H
