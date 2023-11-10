#ifndef VIN_EDITOR_ASSETSHADER_H
#define VIN_EDITOR_ASSETSHADER_H

#include "assetimporter.h"
#include <filesystem>
#include <fstream>
#include "../exec.h"
#include <vin/asset/assetshader.h>

struct AssetShaderImportSettings {
    Vin::ShaderType type{ Vin::ShaderType::Compute };
};

template<>
struct AssetImporter<Vin::AssetType::Shader> {
    Vin::String operator()(Vin::StringView path, EditorImportSettings& editorImportSettings, AssetShaderImportSettings& shaderImportSettings) {
        std::filesystem::path dirPath{ path };
        dirPath.remove_filename();
        std::filesystem::path filePath{ path };
        filePath.replace_extension(".shader");
        std::filesystem::path assetPath{ path };
        assetPath.replace_extension(".vasset");

        Vin::String cmd = editorImportSettings.shaderCompilerPath;
        cmd += " -i \"";
        cmd += path;
        cmd += "\" -o \"";
        cmd += PATH_TO_STRING(dirPath);
        cmd += "\" --optimize --aggregate ";

        switch (shaderImportSettings.type) {
            case Vin::ShaderType::Fragment:
                cmd += "--fragment ";
                break;
            case Vin::ShaderType::Vertex:
                cmd += "--vertex ";
                break;
            case Vin::ShaderType::Compute:
                cmd += "--compute ";
                break;
        }

        Vin::String r = Exec(cmd.c_str());
        if (!r.empty()) {
            Vin::Logger::Err("Error while importing Shader : ", path);
            Vin::Logger::Err(r);
            return "";
        }

        std::fstream file{ filePath, std::fstream::in | std::fstream::binary };
        std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

        file.ignore( std::numeric_limits<std::streamsize>::max() );
        size_t size = file.gcount();
        file.clear();
        file.seekg(0, std::ios_base::beg);

        Vin::ShaderFileHeader assetFileHeader{};
        memcpy(assetFileHeader.header.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
        assetFileHeader.header.type = Vin::AssetType::Shader;
        assetFileHeader.header.size = size;
        assetFileHeader.type = shaderImportSettings.type;

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

#endif //VIN_EDITOR_ASSETSHADER_H
