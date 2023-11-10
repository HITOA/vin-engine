#ifndef VIN_EDITOR_ASSETTEXTURE_H
#define VIN_EDITOR_ASSETTEXTURE_H

#include "assetimporter.h"
#include <filesystem>
#include <fstream>
#include "../exec.h"

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

struct AssetTextureImportSettings {
    TextureEncodingFormat encodingFormat{ TextureEncodingFormat::BC1 };
    TextureEncodingQuality encodingQuality{ TextureEncodingQuality::Default };
    bool mips{ true }; //Generate Mipmaps
    bool linear{ false }; //No gamma correction
    bool pma{ false }; //Premultiply alpha into rgb channel
};

template<>
struct AssetImporter<Vin::AssetType::Texture> {
    Vin::String operator()(Vin::StringView path, EditorImportSettings& editorImportSettings, AssetTextureImportSettings& textureImportSettings) {
        std::filesystem::path filePath{ path };
        filePath.replace_extension(".dds");
        std::filesystem::path assetPath{ path };
        assetPath.replace_extension(".vasset");

        Vin::String cmd = editorImportSettings.textureCompilerPath;
        cmd += " -f \"";
        cmd += path;
        cmd += "\" -o \"";
        cmd += PATH_TO_STRING(filePath);
        cmd += "\" --as .dds ";

        switch (textureImportSettings.encodingFormat) {
            case TextureEncodingFormat::BC1:
                cmd += "-t BC1 ";
                break;
            case TextureEncodingFormat::BC3:
                cmd += "-t BC3 ";
                break;
            case TextureEncodingFormat::BC4:
                cmd += "-t BC4 ";
                break;
            case TextureEncodingFormat::BC5:
                cmd += "-t BC5 ";
                break;
        }

        switch (textureImportSettings.encodingQuality) {
            case TextureEncodingQuality::Default:
                cmd += "-q default ";
                break;
            case TextureEncodingQuality::Fastest:
                cmd += "-q fastest ";
                break;
            case TextureEncodingQuality::Highest:
                cmd += "-q highest ";
                break;
        }

        if (textureImportSettings.mips)
            cmd += "-m ";
        if (textureImportSettings.linear)
            cmd += "--linear ";
        if (textureImportSettings.pma)
            cmd += "--pma ";

        Vin::String r = Exec(cmd.c_str());
        if (!r.empty()) {
            Vin::Logger::Err("Error while importing texture : ", path);
            return "";
        }

        std::fstream file{ filePath, std::fstream::in | std::fstream::binary };
        std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

        file.ignore( std::numeric_limits<std::streamsize>::max() );
        size_t size = file.gcount();
        file.clear();
        file.seekg(0, std::ios_base::beg);

        Vin::AssetFileHeader assetFileHeader{};
        memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
        assetFileHeader.type = Vin::AssetType::Texture;
        assetFileHeader.size = size;

        asset.write((const char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));

        std::vector<char> fileContent{};
        fileContent.resize(size);
        file.read(fileContent.data(), fileContent.size());

        asset.write(fileContent.data(), fileContent.size());

        file.close();
        asset.close();

        std::remove(PATH_TO_STRING(filePath).c_str());

        return PATH_TO_STRING(assetPath);
    }
};

#endif //VIN_EDITOR_ASSETTEXTURE_H
