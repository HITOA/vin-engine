#include "shaderimporter.h"
#include <filesystem>
#include <fstream>
#include "../exec.h"
#include <bgfx/bgfx.h>
#include <regex>

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

static const char* attribNames[] =
        {
                "a_position",
                "a_normal",
                "a_tangent",
                "a_bitangent",
                "a_color0",
                "a_color1",
                "a_color2",
                "a_color3",
                "a_indices",
                "a_weight",
                "a_texcoord0",
                "a_texcoord1",
                "a_texcoord2",
                "a_texcoord3",
                "a_texcoord4",
                "a_texcoord5",
                "a_texcoord6",
                "a_texcoord7",
        };

struct Uniform
{
    static const uint32_t size = sizeof(uint8_t) * 5 + sizeof(uint16_t) * 3;

    Uniform()
            : type(bgfx::UniformType::Count)
            , num(0)
            , regIndex(0)
            , regCount(0)
            , texComponent(0)
            , texDimension(0)
            , texFormat(0)
    {
    }

    Vin::String name;
    bgfx::UniformType::Enum type;
    uint8_t num;
    uint16_t regIndex;
    uint16_t regCount;
    uint8_t texComponent;
    uint8_t texDimension;
    uint16_t texFormat;
};

template<typename T>
static inline size_t WriteInMemory(void* mem, T v) {
    memcpy(mem, &v, sizeof(T));
    return sizeof(T);
}

static inline size_t WriteStringInMemory(void* mem, Vin::String str) {
    memcpy(mem, str.data(), str.size());
    return str.size();
}

static inline bgfx::UniformType::Enum GetBgfxUniformType(unsigned int type) {
    switch (type) {
        case 0x1404:
            return bgfx::UniformType::Sampler;
        case 0x8B52:
            return bgfx::UniformType::Vec4;
        case 0x8B5B:
            return bgfx::UniformType::Mat3;
        case 0x8B5C:
            return bgfx::UniformType::Mat4;
        default:
            return bgfx::UniformType::Count;
    }
}

nlohmann::json ShaderImporter::ImportSettings::Serialize() const {
    nlohmann::json serializedData{};

    serializedData["Type"] = (int)type;

    return serializedData;
}

void ShaderImporter::ImportSettings::Deserialize(const nlohmann::json &data) {
    if (data.count("Type"))
        type = (Vin::ShaderType)data["Type"].get<int>();
}

Vin::AssetType ShaderImporter::GetType() const {
    return Vin::AssetType::Shader;
}

bool ShaderImporter::Match(Vin::StringView path) const {
    static const char* shaderExt = ".compute.frag.vert";
    std::filesystem::path ext{ path };

    return std::string{ shaderExt }.find(ext.extension().c_str()) != std::string::npos;
}

AssetRegistryID ShaderImporter::ImportFromFile(Vin::StringView path, const EditorImportSettings &editorImportSettings) {
    ImportSettings shaderImportSettings{};
    shaderImportSettings.type = GetShaderStage(path);

    std::filesystem::path assetPath{ path };
    assetPath.replace_extension(".vasset");

    Vin::Vector<char> compiledShader{};
    size_t s = CompileShader(path, editorImportSettings, shaderImportSettings, compiledShader);
    if (!s)
        return "";

    std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

    Vin::ShaderFileHeader assetFileHeader{};
    memcpy(assetFileHeader.header.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
    assetFileHeader.header.type = Vin::AssetType::Shader;
    assetFileHeader.header.depCount = 0;
    assetFileHeader.header.size = s;
    assetFileHeader.type = shaderImportSettings.type;

    asset.write((char*)&assetFileHeader, sizeof(Vin::ShaderFileHeader));
    asset.write(compiledShader.data(), s);
    asset.close();

    AssetRegistryEntry assetEntry{};
    assetEntry.type = Vin::AssetType::Shader;
    assetEntry.metadata["Import Settings"] = shaderImportSettings.Serialize();
    assetEntry.metadata["Imported Asset Path"] = PATH_TO_STRING(std::filesystem::relative(assetPath, project->GetWorkingDirectory()));

    AssetRegistryID assetId = PATH_TO_STRING(std::filesystem::relative(path, project->GetWorkingDirectory()));
    project->GetAssetRegistry()->AddAsset(assetId, assetEntry);

    return assetId;
}

AssetRegistryID ShaderImporter::ImportFromMemory(char *data, size_t size, Vin::Vector<char>& out,
                                                 const EditorImportSettings &editorImportSettings) {
    return "";
}

bool ShaderImporter::ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings &editorImportSettings) {
    return false;
}

size_t ShaderImporter::CompileShader(Vin::StringView path, const EditorImportSettings &editorImportSettings,
                                     const ShaderImporter::ImportSettings &shaderImportSettings,
                                     Vin::Vector<char> &out) {
    std::filesystem::path filePath{ std::filesystem::temp_directory_path() };
    filePath /= std::filesystem::path{ path }.filename();
    filePath.replace_extension(".shader");
    std::filesystem::path reflectionPath{ std::filesystem::temp_directory_path()  };
    reflectionPath /= std::filesystem::path{ path }.filename();;
    reflectionPath.replace_extension(".info.json");

    Vin::String cmd = editorImportSettings.shaderCompilerPath;
    cmd += " -i \"";
    cmd += path;
    cmd += "\" -o \"";
    cmd += PATH_TO_STRING(std::filesystem::temp_directory_path());
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

    cmd += "--sysinclude \"";
    cmd += editorImportSettings.shaderLibraryPath;
    cmd += "\" ";

    cmd += "--locinclude \"";
    cmd += project->GetWorkingDirectory();
    cmd += "\" ";

    Vin::String r = Exec(cmd.c_str());
    if (!r.empty()) {
        Vin::Logger::Err("Error while importing Shader : ", path);
        Vin::Logger::Err(r);
        return 0;
    }

    std::fstream file{ filePath, std::fstream::in | std::fstream::binary };
    std::fstream ref{ reflectionPath, std::fstream::in | std::fstream::binary };

    file.ignore( std::numeric_limits<std::streamsize>::max() );
    size_t size = file.gcount();
    file.clear();
    file.seekg(0, std::ios_base::beg);

    std::vector<char> fileContent{};
    fileContent.resize(size);
    file.read(fileContent.data(), fileContent.size());
    file.close();

    nlohmann::json reflection = nlohmann::json::parse(ref);
    ref.close();

    ShaderCC::AggregateShaderFileHeader aggregateShaderFileHeader{};
    ShaderCC::ReadAggregateShaderFileHeader(fileContent.data(), fileContent.size(), aggregateShaderFileHeader);

    Vin::Vector<char> glslSource{};
    Vin::Vector<char> spirvSource{};
    Vin::Vector<char> hlslSource{};
    Vin::Vector<char> mslSource{};

    ProcessShaderCode(glslSource, reflection["glsl"], fileContent.data() + aggregateShaderFileHeader.glsl.position,
                aggregateShaderFileHeader.glsl.size, shaderImportSettings.type, true);

    ProcessShaderCode(spirvSource, reflection["spirv"], fileContent.data() + aggregateShaderFileHeader.spirv.position,
                aggregateShaderFileHeader.spirv.size, shaderImportSettings.type);

    ProcessShaderCode(hlslSource, reflection["spirv"], fileContent.data() + aggregateShaderFileHeader.hlsl.position,
                aggregateShaderFileHeader.hlsl.size, shaderImportSettings.type);

    ProcessShaderCode(mslSource, reflection["spirv"], fileContent.data() + aggregateShaderFileHeader.msl.position,
                aggregateShaderFileHeader.msl.size, shaderImportSettings.type);

    aggregateShaderFileHeader.glsl.size = glslSource.size();
    aggregateShaderFileHeader.spirv.position = aggregateShaderFileHeader.glsl.position + aggregateShaderFileHeader.glsl.size;
    aggregateShaderFileHeader.spirv.size = spirvSource.size();
    aggregateShaderFileHeader.hlsl.position = aggregateShaderFileHeader.spirv.position + aggregateShaderFileHeader.spirv.size;
    aggregateShaderFileHeader.hlsl.size = hlslSource.size();
    aggregateShaderFileHeader.msl.position = aggregateShaderFileHeader.hlsl.position + aggregateShaderFileHeader.hlsl.size;
    aggregateShaderFileHeader.msl.size = mslSource.size();

    out.resize(aggregateShaderFileHeader.msl.position + aggregateShaderFileHeader.msl.size);
    memcpy(out.data(), &aggregateShaderFileHeader, sizeof(ShaderCC::AggregateShaderFileHeader));
    memcpy(out.data() + aggregateShaderFileHeader.glsl.position, glslSource.data(), glslSource.size());
    memcpy(out.data() + aggregateShaderFileHeader.spirv.position, spirvSource.data(), spirvSource.size());
    memcpy(out.data() + aggregateShaderFileHeader.hlsl.position, hlslSource.data(), hlslSource.size());
    memcpy(out.data() + aggregateShaderFileHeader.msl.position, mslSource.data(), mslSource.size());

    return out.size();
}

void ShaderImporter::ProcessShaderCode(Vin::Vector<char> &out, nlohmann::json &reflection, const char *source, size_t size,
                                  Vin::ShaderType type, bool glslRenameAttribute) {

    int attributeCount = reflection["attributes"].size();
    uint16_t uniformsCount = reflection["uniforms"].size();

    Vin::Vector<char, Vin::Core::AllocationStrategy::SingleFrame> shaderBgfxHeader{};
    shaderBgfxHeader.resize(14);

    switch (type) {
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
    memcpy(&shaderBgfxHeader[12], &uniformsCount, sizeof(uint16_t));
    size_t c = shaderBgfxHeader.size();
    uint16_t bsize = 0;

    uint32_t fragmentBit = type == Vin::ShaderType::Fragment ? 0x10 : 0;

    for (int i = 0; i < uniformsCount; ++i) {
        Uniform uniform{};
        uniform.name = Vin::String{ reflection["uniforms"][i]["name"].get<std::string>() };
        uniform.num = reflection["uniforms"][i]["arraySize"].get<int>();
        uniform.regIndex = reflection["uniforms"][i]["offset"].get<int>();
        uniform.regCount = uniform.num;
        uniform.type = GetBgfxUniformType(reflection["uniforms"][i]["type"].get<int>());

        switch (uniform.type) {
            case bgfx::UniformType::Mat3:
                uniform.regCount *= 3;
                break;
            case bgfx::UniformType::Mat4:
                uniform.regCount *= 4;
                break;
            default:
                break;
        }

        if ( (uniform.type & ~(0x10 | 0x20 | 0x40 | 0x80)) > bgfx::UniformType::End)
            bsize = bsize > (uint16_t)(uniform.regIndex + uniform.regCount*16) ? bsize : (uint16_t)(uniform.regIndex + uniform.regCount*16);

        shaderBgfxHeader.resize(shaderBgfxHeader.size() + Uniform::size + uniform.name.size());
        uint8_t nameSize = uniform.name.size();
        c += WriteInMemory(&shaderBgfxHeader[c], nameSize);
        c += WriteStringInMemory(&shaderBgfxHeader[c], uniform.name);
        c += WriteInMemory(&shaderBgfxHeader[c], (uint8_t)(uniform.type | fragmentBit));
        c += WriteInMemory(&shaderBgfxHeader[c], uniform.num);
        c += WriteInMemory(&shaderBgfxHeader[c], uniform.regIndex);
        c += WriteInMemory(&shaderBgfxHeader[c], uniform.regCount);
        c += WriteInMemory(&shaderBgfxHeader[c], uniform.texComponent);
        c += WriteInMemory(&shaderBgfxHeader[c], uniform.texDimension);
        c += WriteInMemory(&shaderBgfxHeader[c], uniform.texFormat);
    }

    shaderBgfxHeader.resize(shaderBgfxHeader.size() + 4);

    Vin::BasicString<char, Vin::Core::AllocationStrategy::SingleFrame> sourceStr{ source, size };

    Vin::Vector<char, Vin::Core::AllocationStrategy::SingleFrame> shaderBgfxFooter{};
    shaderBgfxFooter.resize(4 + 2 * attributeCount);
    shaderBgfxFooter[0] = 0;
    shaderBgfxFooter[1] = (char)attributeCount;

    for (int i = 0; i < attributeCount; ++i) {
        if (glslRenameAttribute) {
            const char* attribName = attribNames[reflection["attributes"][i]["location"].get<int>()];
            sourceStr = std::regex_replace(sourceStr,
                                           std::regex{"\\b" + reflection["attributes"][i]["name"].get<std::string>() + "\\b"},
                                           attribName);
        }
        uint16_t attribId = attribIds[reflection["attributes"][i]["location"].get<int>()];
        memcpy(&shaderBgfxFooter[2 + 2 * i], &attribId, 2);
    }

    memcpy(shaderBgfxFooter.data() + shaderBgfxFooter.size() - 2, &bsize, 2);

    uint32_t shaderSize = sourceStr.size();
    memcpy(shaderBgfxHeader.data() + shaderBgfxHeader.size() - 4, &shaderSize, 4);

    out.resize(shaderBgfxHeader.size() + sourceStr.size() + shaderBgfxFooter.size());
    memcpy(out.data(), shaderBgfxHeader.data(), shaderBgfxHeader.size());
    memcpy(out.data() + shaderBgfxHeader.size(), sourceStr.data(), sourceStr.size());
    memcpy(out.data() + shaderBgfxHeader.size() + sourceStr.size(), shaderBgfxFooter.data(), shaderBgfxFooter.size());
}

Vin::ShaderType ShaderImporter::GetShaderStage(Vin::StringView path) {
    if (memcmp(&path.data()[path.size() - 5], ".frag", 4) == 0)
        return Vin::ShaderType::Fragment;

    if (memcmp(&path.data()[path.size() - 5], ".vert", 4) == 0)
        return Vin::ShaderType::Vertex;

    return Vin::ShaderType::Compute;
}