#ifndef VIN_EDITOR_ASSETSHADER_H
#define VIN_EDITOR_ASSETSHADER_H

#include "assetimporter.h"
#include <filesystem>
#include <fstream>
#include "../exec.h"
#include <vin/asset/assetshader.h>
#include <vin/asset/aggregateshader.h>
#include <vin/json/json.hpp>

//Bgfx location to attribIds mapping
static uint16_t attribIds[] = {
        0x0001,
        0x0002,
        0x0003,
        0x0004,
        0x0005,
        0x0006,
        0x0018,
        0x0019,
        0x000e,
        0x000f,
        0x0010,
        0x0011,
        0x0012,
        0x0013,
        0x0014,
        0x0015,
        0x0016,
        0x0017
};

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
        std::filesystem::path reflectionPath{ path };
        reflectionPath.replace_extension(".info.json");
        std::filesystem::path assetPath{ path };
        assetPath.replace_extension(".vasset");

        Vin::String cmd = editorImportSettings.shaderCompilerPath;
        cmd += " -i \"";
        cmd += path;
        cmd += "\" -o \"";
        cmd += PATH_TO_STRING(dirPath);
        cmd += "\" --optimize --aggregate --info ";

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
        std::fstream ref{ reflectionPath, std::fstream::in | std::fstream::binary };
        std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

        file.ignore( std::numeric_limits<std::streamsize>::max() );
        size_t size = file.gcount();
        file.clear();
        file.seekg(0, std::ios_base::beg);

        std::vector<char> fileContent{};
        fileContent.resize(size);
        file.read(fileContent.data(), fileContent.size());

        nlohmann::json reflection = nlohmann::json::parse(ref);

        int attributeCount = reflection["spirv"]["attributes"].size();

        std::vector<char> shaderBgfxHeader{};
        shaderBgfxHeader.resize(18);

        switch (shaderImportSettings.type) {
            case Vin::ShaderType::Fragment:
                memcpy(shaderBgfxHeader.data(), "FSH", 3);
                break;
            case Vin::ShaderType::Vertex:
                memcpy(shaderBgfxHeader.data(), "VSH", 3);
                break;
            case Vin::ShaderType::Compute:
                memcpy(shaderBgfxHeader.data(), "CSH", 3);
                break;
        }

        shaderBgfxHeader[3] = 11;
        memset(shaderBgfxHeader.data() + 4, 0, 10);

        std::vector<char> shaderBgfxFooter{};
        shaderBgfxFooter.resize(4 + 2 * attributeCount);
        shaderBgfxFooter[0] = 0;
        shaderBgfxFooter[1] = (char)attributeCount;

        for (int i = 0; i < attributeCount; ++i) {
            uint16_t attribId = attribIds[reflection["spirv"]["attributes"][i]["location"].get<int>()];
            memcpy(&shaderBgfxFooter[2 + 2 * i], &attribId, 2);
        }

        bgfx::Attrib

        memset(shaderBgfxFooter.data() + shaderBgfxFooter.size() - 2, 0, 2);

        size_t bgfxAdditionalSize = shaderBgfxHeader.size() + shaderBgfxFooter.size();

        ShaderCC::AggregateShaderFileHeader aggregateShaderFileHeader{};
        ShaderCC::ReadAggregateShaderFileHeader(fileContent.data(), fileContent.size(), aggregateShaderFileHeader);
        ShaderCC::AggregateShaderFileHeader newAggregateShaderFileHeader = aggregateShaderFileHeader;
        newAggregateShaderFileHeader.glsl.size += bgfxAdditionalSize;
        newAggregateShaderFileHeader.spirv.position = newAggregateShaderFileHeader.glsl.position + newAggregateShaderFileHeader.glsl.size;
        newAggregateShaderFileHeader.spirv.size += bgfxAdditionalSize;
        newAggregateShaderFileHeader.hlsl.position = newAggregateShaderFileHeader.spirv.position + newAggregateShaderFileHeader.spirv.size;
        newAggregateShaderFileHeader.hlsl.size += bgfxAdditionalSize;
        newAggregateShaderFileHeader.msl.position = newAggregateShaderFileHeader.hlsl.position + newAggregateShaderFileHeader.hlsl.size;
        newAggregateShaderFileHeader.msl.size += bgfxAdditionalSize;

        Vin::ShaderFileHeader assetFileHeader{};
        memcpy(assetFileHeader.header.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
        assetFileHeader.header.type = Vin::AssetType::Shader;
        assetFileHeader.header.size = newAggregateShaderFileHeader.msl.position + newAggregateShaderFileHeader.msl.size;
        assetFileHeader.type = shaderImportSettings.type;

        asset.write((const char*)&assetFileHeader, sizeof(Vin::ShaderFileHeader));

        asset.write((const char*)&newAggregateShaderFileHeader, sizeof(ShaderCC::AggregateShaderFileHeader));

        uint32_t shaderSize = aggregateShaderFileHeader.glsl.size;
        memcpy(shaderBgfxHeader.data() + 14, &shaderSize, 4);
        asset.write(shaderBgfxHeader.data(), shaderBgfxHeader.size());
        asset.write(fileContent.data() + aggregateShaderFileHeader.glsl.position, aggregateShaderFileHeader.glsl.size);
        asset.write(shaderBgfxFooter.data(), shaderBgfxFooter.size());

        shaderSize = aggregateShaderFileHeader.spirv.size;
        memcpy(shaderBgfxHeader.data() + 14, &shaderSize, 4);
        asset.write(shaderBgfxHeader.data(), shaderBgfxHeader.size());
        asset.write(fileContent.data() + aggregateShaderFileHeader.spirv.position, aggregateShaderFileHeader.spirv.size);
        asset.write(shaderBgfxFooter.data(), shaderBgfxFooter.size());

        shaderSize = aggregateShaderFileHeader.hlsl.size;
        memcpy(shaderBgfxHeader.data() + 14, &shaderSize, 4);
        asset.write(shaderBgfxHeader.data(), shaderBgfxHeader.size());
        asset.write(fileContent.data() + aggregateShaderFileHeader.hlsl.position, aggregateShaderFileHeader.hlsl.size);
        asset.write(shaderBgfxFooter.data(), shaderBgfxFooter.size());

        shaderSize = aggregateShaderFileHeader.msl.size;
        memcpy(shaderBgfxHeader.data() + 14, &shaderSize, 4);
        asset.write(shaderBgfxHeader.data(), shaderBgfxHeader.size());
        asset.write(fileContent.data() + aggregateShaderFileHeader.msl.position, aggregateShaderFileHeader.msl.size);
        asset.write(shaderBgfxFooter.data(), shaderBgfxFooter.size());

        file.close();
        asset.close();

        std::remove(PATH_TO_STRING(filePath).c_str());
        std::remove(PATH_TO_STRING(reflectionPath).c_str());

        return PATH_TO_STRING(assetPath);
    }
};

#endif //VIN_EDITOR_ASSETSHADER_H
