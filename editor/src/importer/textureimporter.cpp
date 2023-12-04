#include "textureimporter.h"
#include <filesystem>
#include <fstream>
#include "../exec.h"

nlohmann::json TextureImporter::ImportSettings::Serialize() const {
    nlohmann::json serializedData{};
    serializedData["Encoding Format"] = (int)encodingFormat;
    serializedData["Encoding Quality"] = (int)encodingQuality;
    serializedData["Mips"] = mips;
    serializedData["Linear"] = linear;
    serializedData["Premultiply Alpha"] = pma;
    return serializedData;
}

void TextureImporter::ImportSettings::Deserialize(const nlohmann::json &data) {
    if (data.count("Encoding Format"))
        encodingFormat = (TextureEncodingFormat)data["Encoding Format"].get<int>();
    if (data.count("Encoding Quality"))
        encodingQuality = (TextureEncodingQuality)data["Encoding Quality"].get<int>();
    if (data.count("Mips"))
        mips = data["Mips"].get<bool>();
    if (data.count("Linear"))
        linear = data["Linear"].get<bool>();
    if (data.count("Premultiply Alpha"))
        pma = data["Premultiply Alpha"].get<bool>();
}

Vin::AssetType TextureImporter::GetType() const {
    return Vin::AssetType::Texture;
}

bool TextureImporter::Match(Vin::StringView path) const {
    static const char* textureExt = ".bmp.dds.exr.gif.jpg.jpeg.hdr.ktx.png.psd.pvr.tga";
    std::filesystem::path ext{ path };

    return std::string{ textureExt }.find(ext.extension().c_str()) != std::string::npos;
}

AssetRegistryID TextureImporter::ImportFromFile(Vin::StringView path, const EditorImportSettings& editorImportSettings) {
    std::filesystem::path assetPath{ path };
    assetPath.replace_extension(".vasset");

    Vin::Vector<char> compiledTexture{};
    size_t s = CompileTexture(path, editorImportSettings, textureImportSettings, compiledTexture);
    if (!s)
        return "";

    std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

    Vin::AssetFileHeader assetFileHeader{};
    memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
    assetFileHeader.type = Vin::AssetType::Texture;
    assetFileHeader.depCount = 0;
    assetFileHeader.size = s;

    asset.write((char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));
    asset.write(compiledTexture.data(), s);
    asset.close();

    AssetRegistryEntry assetEntry{};
    assetEntry.type = Vin::AssetType::Texture;
    assetEntry.metadata["Import Settings"] = textureImportSettings.Serialize();
    assetEntry.metadata["Imported Asset Path"] = PATH_TO_STRING(std::filesystem::relative(assetPath, project->GetWorkingDirectory()));

    AssetRegistryID assetId = PATH_TO_STRING(std::filesystem::relative(path, project->GetWorkingDirectory()));
    project->GetAssetRegistry()->AddAsset(assetId, assetEntry);

    return assetId;
}

AssetRegistryID TextureImporter::ImportFromMemory(char *data, size_t size, Vin::Vector<char>& out, const EditorImportSettings& editorImportSettings) {
    std::filesystem::path filePath{ std::filesystem::temp_directory_path() / Vin::GenerateRandomNameId(8) };
    std::fstream file{ filePath, std::fstream::out | std::fstream::trunc | std::fstream::binary };
    file.write(data, size);
    file.close();

    Vin::Vector<char> compiledTexture{};
    size_t s = CompileTexture(PATH_TO_STRING(filePath), editorImportSettings, textureImportSettings, compiledTexture);
    if (!s)
        return "";

    Vin::AssetFileHeader assetFileHeader{};
    memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
    assetFileHeader.type = Vin::AssetType::Texture;
    assetFileHeader.depCount = 0;
    assetFileHeader.size = s;

    out.resize(sizeof(Vin::AssetFileHeader) + s);
    memcpy(out.data(), &assetFileHeader, sizeof(Vin::AssetFileHeader));
    memcpy(out.data() + sizeof(Vin::AssetFileHeader), compiledTexture.data(), s);

    AssetRegistryEntry assetEntry{};
    assetEntry.type = Vin::AssetType::Texture;
    assetEntry.isOrphan = true;
    assetEntry.metadata["Import Settings"] = textureImportSettings.Serialize();

    AssetRegistryID assetId = Vin::GenerateRandomNameId(32);
    project->GetAssetRegistry()->AddAsset(assetId, assetEntry);

    std::remove(PATH_TO_STRING(filePath).c_str());

    return assetId;
}

bool TextureImporter::ReimportAsset(AssetRegistryID assetRegistryId, const EditorImportSettings& editorImportSettings) {
    AssetRegistryEntry& entry = project->GetAssetRegistry()->GetAsset(assetRegistryId);
    std::filesystem::path assetPath{ project->GetWorkingDirectory() };
    assetPath /= assetRegistryId;
    assetPath.replace_extension(".vasset");
    ImportSettings currTextureImportSettings{};
    if (entry.isOrphan) {
        Vin::Logger::Err("Can't reimport orphan texture.");
        return false;
    }
    if (entry.metadata.count("Import Settings"))
        currTextureImportSettings.Deserialize(entry.metadata["Import Settings"]);
    if (entry.metadata.count("Imported Asset Path")) {
        assetPath = project->GetWorkingDirectory();
        assetPath /= entry.metadata["Imported Asset Path"].get<std::string>();
    }


    Vin::Vector<char> compiledTexture{};
    size_t s = CompileTexture(assetRegistryId, editorImportSettings, textureImportSettings, compiledTexture);
    if (!s)
        return "";

    std::fstream asset{ assetPath, std::fstream::out | std::fstream::trunc | std::fstream::binary };

    Vin::AssetFileHeader assetFileHeader{};
    memcpy(assetFileHeader.magic, ASSET_FILE_MAGIC, sizeof(ASSET_FILE_MAGIC));
    assetFileHeader.type = Vin::AssetType::Texture;
    assetFileHeader.depCount = 0;
    assetFileHeader.size = s;

    asset.write((char*)&assetFileHeader, sizeof(Vin::AssetFileHeader));
    asset.write(compiledTexture.data(), s);
    asset.close();

    return true;
}

void TextureImporter::SetImportSettings(const TextureImporter::ImportSettings& textureImportSettings) {
    this->textureImportSettings = textureImportSettings;
}

TextureImporter::ImportSettings TextureImporter::GetImportSettings() {
    return textureImportSettings;
}

size_t TextureImporter::CompileTexture(Vin::StringView path, const EditorImportSettings &editorImportSettings, const ImportSettings& textureImportSettings, Vin::Vector<char>& out) {
    std::filesystem::path filePath{ std::filesystem::temp_directory_path() / Vin::GenerateRandomNameId(8) };
    filePath += ".dds";

    Vin::String cmd = editorImportSettings.textureCompilerPath;
    cmd += " -f \"";
    cmd += PATH_TO_STRING(std::filesystem::absolute(path));
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
        Vin::Logger::Err("Error while compiling texture : ", path);
        return 0;
    }

    std::fstream file{ filePath, std::fstream::in | std::fstream::binary };

    file.ignore( std::numeric_limits<std::streamsize>::max() );
    size_t size = file.gcount();
    file.clear();
    file.seekg(0, std::ios_base::beg);

    out.resize(size);
    file.read(out.data(), size);
    file.close();

    std::remove(PATH_TO_STRING(filePath).c_str());

    return size;
}

