#include "programimporter.h"
#include <filesystem>
#include <fstream>
#include <vin/core/templates/scratch.h>

Vin::AssetType ProgramImporter::GetType() const {
    return Vin::AssetType::Program;
}

bool ProgramImporter::Match(Vin::StringView path) const {
    static const char* shaderExt = ".shader.program";
    std::filesystem::path ext{ path };

    return std::string{ shaderExt }.find(ext.extension().c_str()) != std::string::npos;
}

AssetRegistryID ProgramImporter::ImportFromFile(Vin::StringView path, const EditorImportSettings &editorImportSettings) {
    std::filesystem::path fullPath = std::filesystem::absolute(path);
    std::filesystem::path assetPath = std::filesystem::absolute(path);
    assetPath.replace_extension(".vasset");

    std::fstream file{ fullPath, std::fstream::in | std::fstream::binary };
    file.ignore( std::numeric_limits<std::streamsize>::max() );
    size_t size = file.gcount();
    file.clear();
    file.seekg(0, std::ios_base::beg);

    Vin::Vector<char> fileContent{};
    fileContent.resize(size);
    file.read(fileContent.data(), fileContent.size());
    file.close();

    Vin::Vector<Token> tokens = Tokenize(fileContent.data(), fileContent.size());

    std::filesystem::path vertexPath{ std::filesystem::temp_directory_path() / Vin::GenerateRandomNameId(8) };
    vertexPath += ".vert";
    std::filesystem::path fragmentPath{ std::filesystem::temp_directory_path() / Vin::GenerateRandomNameId(8) };
    fragmentPath += ".frag";

    std::fstream vertexFile{ vertexPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };
    std::fstream fragmentFile{ fragmentPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

    vertexFile.write("#version 450\n#extension GL_GOOGLE_include_directive : require\n", 62);
    fragmentFile.write("#version 450\n#extension GL_GOOGLE_include_directive : require\n", 62);

    nlohmann::json programMetadata{};

    {
        int i = 0;
        while (i < tokens.size()) {
            switch(tokens[i].type) {
                case VinShaderToken::Vertex:
                    vertexFile.write(tokens[i].token.begin(), tokens[i].token.size());
                    ++i;
                    break;
                case VinShaderToken::Fragment:
                    fragmentFile.write(tokens[i].token.begin(), tokens[i].token.size());
                    ++i;
                    break;
                case VinShaderToken::Identifier: {
                    int c = ParseField(tokens, i, programMetadata);
                    if (c == 0)
                        ++i;
                    i += c;
                    break;
                }
                default:
                    ++i;
                    break;
            }
        }
    }
    vertexFile.close();
    fragmentFile.close();

    ShaderImporter shaderImporter{};
    shaderImporter.project = project;

    Vin::Vector<char> compiledVertexShader{};
    Vin::Vector<char> compiledFragmentShader{};

    size_t vS = shaderImporter.CompileShader(PATH_TO_STRING(vertexPath), editorImportSettings,
                                             ShaderImporter::ImportSettings{ Vin::ShaderType::Vertex }, compiledVertexShader);
    size_t fS = shaderImporter.CompileShader(PATH_TO_STRING(fragmentPath), editorImportSettings,
                                             ShaderImporter::ImportSettings{ Vin::ShaderType::Fragment }, compiledFragmentShader);
    if (!vS || !fS)
        return "";

    Vin::ScratchBuffer<> buffer{};

    buffer.Write<uint16_t>(0);
    buffer.Write<uint32_t>(compiledVertexShader.size());
    buffer.Write<uint32_t>(compiledFragmentShader.size());
    buffer.Write((uint8_t*)compiledVertexShader.data(), compiledVertexShader.size());
    buffer.Write((uint8_t*)compiledFragmentShader.data(), compiledFragmentShader.size());

    std::fstream assetFile{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

    Vin::AssetFileHeader assetFileHeader{};
    memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
    assetFileHeader.type = Vin::AssetType::Program;
    assetFileHeader.depCount = 0;
    assetFileHeader.size = buffer.GetSize();

    assetFile.write((char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));
    assetFile.write((char*)buffer.GetData(), buffer.GetSize());
    assetFile.close();

    AssetRegistryEntry assetEntry{};
    assetEntry.type = Vin::AssetType::Program;
    //assetEntry.metadata["Import Settings"] = programImportSettings.Serialize();
    assetEntry.metadata["Program"] = programMetadata;
    assetEntry.metadata["Imported Asset Path"] = PATH_TO_STRING(std::filesystem::relative(assetPath, project->GetWorkingDirectory()));

    AssetRegistryID assetId = PATH_TO_STRING(std::filesystem::relative(path, project->GetWorkingDirectory()));
    project->GetAssetRegistry()->AddAsset(assetId, assetEntry);

    return "";
}

AssetRegistryID ProgramImporter::ImportFromMemory(char *data, size_t size, Vin::Vector<char> &out,
                                                  const EditorImportSettings &editorImportSettings) {
    return "";
}

bool ProgramImporter::ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings &editorImportSettings) {
    return false;
}

Vin::Vector<ProgramImporter::Token> ProgramImporter::Tokenize(const char *data, size_t size) {
    static const char* vertexBegin = "GLSLVERTEXBEGIN";
    static const char* vertexEnd = "GLSLVERTEXEND";
    static const char* fragmentBegin = "GLSLFRAGMENTBEGIN";
    static const char* fragmentEnd = "GLSLFRAGMENTEND";

    Vin::Vector<Token> tokens{};
    bool vertexShader{ false };
    bool fragmentShader{ false };

    //Very simple and wanky tokenizer but it works

    int c{ 0 };
    while (c < size) {
        while (isspace(data[c]))
            ++c;

        if (memcmp(&data[c], vertexBegin, 15) == 0 && c + 15 < size) {
            if (vertexShader)
                Vin::Logger::Warn("Multiple vertex stage is present. only one should be.");
            c += 15;
            int l = 0;
            while (c + l + 13 < size && memcmp(&data[c + l], vertexEnd, 13))
                ++l;
            tokens.emplace_back(Vin::StringView{ &data[c], (size_t)l }, VinShaderToken::Vertex);
            c += l + 13;
            vertexShader = true;
            continue;
        }

        if (memcmp(&data[c], fragmentBegin, 17) == 0 && c + 17 < size) {
            if (fragmentShader)
                Vin::Logger::Warn("Multiple Fragment stage is present. only one should be.");
            c += 17;
            int l = 0;
            while (c + l + 15 < size && memcmp(&data[c + l], fragmentEnd, 15))
                ++l;
            tokens.emplace_back(Vin::StringView{ &data[c], (size_t)l }, VinShaderToken::Fragment);
            c += l + 15;
            fragmentShader = true;
            continue;
        }

        if (isalpha(data[c]) || data[c] == '_') {
            int l = 0;
            while ((isalnum(data[c + l]) || data[c + l] == '_') && c + l < size)
                ++l;
            tokens.emplace_back(Vin::StringView{ &data[c], (size_t)l }, VinShaderToken::Identifier);
            c += l;
            continue;
        }

        if (isdigit(data[c])) {
            int l = 0;
            while ((isdigit(data[c + l]) || data[c + l] == '.') && c + l < size)
                ++l;
            tokens.emplace_back(Vin::StringView{ &data[c], (size_t)l }, VinShaderToken::Number);
            c += l;
            continue;
        }

        if (data[c] == '"') {
            ++c;
            int l = 0;
            while (data[c + l] != '"' && c + l < size)
                ++l;
            tokens.emplace_back(Vin::StringView{ &data[c], (size_t)l }, VinShaderToken::String);
            c += l + 1;
            continue;
        }
        switch (data[c]) {
            case '=':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::Equal);
                ++c;
                break;
            case ':':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::Colon);
                ++c;
                break;
            case ',':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::Coma);
                ++c;
                break;
            case '(':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::LPar);
                ++c;
                break;
            case ')':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::RPar);
                ++c;
                break;
            case '{':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::BlkStart);
                ++c;
                break;
            case '}':
                tokens.emplace_back(Vin::StringView{ &data[c], 1 }, VinShaderToken::BlkEnd);
                ++c;
                break;
            default:
                Vin::Logger::Err("Vin Shader syntax error, bad value : ", Vin::StringView{ &data[c], 1 });
                return tokens;
        }
    }

    if (!vertexShader)
        Vin::Logger::Err("Vertex stage is missing.");
    if (!fragmentShader)
        Vin::Logger::Err("Fragment stage is missing.");

    return tokens;
}

int ProgramImporter::ParseField(const Vin::Vector<Token> &tokens, int startIdx, nlohmann::json& programMetadata) {
    //Super giga wanky parser but it's ok

    Vin::StringView propertyName = tokens[startIdx].token;
    if (tokens[startIdx + 1].type != VinShaderToken::Equal) {
        Vin::Logger::Err("Syntax error, equal expected after : ", propertyName);
        return 0;
    }
    startIdx += 2;
    switch (tokens[startIdx].type) {
        case VinShaderToken::String: {
            programMetadata[propertyName] = tokens[startIdx].token;
            return 3;
            break;
        }
        case VinShaderToken::Number: {
            double v = strtod(tokens[startIdx].token.begin(), nullptr);
            programMetadata[propertyName] = v;
            return 3;
        }
        case VinShaderToken::BlkStart: {
            int l = 0;
            ++startIdx;
            nlohmann::json curr{};
            while (startIdx + l < tokens.size() && tokens[startIdx + l].type != VinShaderToken::BlkEnd) {
                if (tokens[startIdx + l].type == VinShaderToken::Identifier) {
                    int c = ParseField(tokens, startIdx + l, curr);
                    if (c == 0)
                        ++l;
                    l += c;
                }
            }
            ++l;
            programMetadata[propertyName] = curr;
            return 3 + l;
            break;
        }
        case VinShaderToken::LPar: {
            int l = 0;
            ++startIdx;
            while (startIdx + l < tokens.size() && tokens[startIdx + l].type != VinShaderToken::RPar) {
                switch (tokens[startIdx + l].type) {
                    case VinShaderToken::String: {
                        programMetadata[propertyName] += tokens[startIdx + l].token;
                        ++l;
                        break;
                    }
                    case VinShaderToken::Number: {
                        double v = strtod(tokens[startIdx + l].token.begin(), nullptr);
                        programMetadata[propertyName] += v;
                        ++l;
                        break;
                    }
                    default:
                        ++l;
                        break;
                }
            }
            ++l;
            return 3 + l;
            break;
        }
        default:
            break;
    }
    return 0;
}