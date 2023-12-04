#include "textureinspector.h"
#include <imgui.h>

TextureInspector::TextureInspector(AssetRegistryID id, EditorModule* editorModule) : id{ id }, editorModule{ editorModule } {
    AssetRegistryEntry& entry = editorModule->GetProject()->GetAssetRegistry()->GetAsset(id);
    if (entry.type == Vin::AssetType::Texture)
        if (entry.metadata.count("Import Settings"))
            textureImportSettings.Deserialize(entry.metadata["Import Settings"]);
}

void TextureInspector::Draw() {
    ImGui::TextUnformatted(id.c_str());

    if (ImGui::BeginCombo("Import Preset", "...")) {
        if (ImGui::Selectable("RGB Texture")) {
            textureImportSettings = TextureImporter::ImportSettings{};
        }

        if (ImGui::Selectable("RGBA Texture")) {
            textureImportSettings = TextureImporter::ImportSettings{};
            textureImportSettings.pma = true;
            textureImportSettings.encodingFormat = TextureEncodingFormat::BC3;
        }

        if (ImGui::Selectable("Normal Map")) {
            textureImportSettings = TextureImporter::ImportSettings{};
            textureImportSettings.linear = true;
            textureImportSettings.encodingFormat = TextureEncodingFormat::BC4;
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (ImGui::BeginCombo("Encoding Format", GetTextureEncodingFormatString(textureImportSettings.encodingFormat).c_str())) {
        if (ImGui::Selectable("BC1"))
            textureImportSettings.encodingFormat = TextureEncodingFormat::BC1;
        if (ImGui::Selectable("BC3"))
            textureImportSettings.encodingFormat = TextureEncodingFormat::BC3;
        if (ImGui::Selectable("BC4"))
            textureImportSettings.encodingFormat = TextureEncodingFormat::BC4;
        if (ImGui::Selectable("BC5"))
            textureImportSettings.encodingFormat = TextureEncodingFormat::BC5;
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Encoding Quality", GetTextureEncodingQualityString(textureImportSettings.encodingQuality).c_str())) {
        if (ImGui::Selectable("Default"))
            textureImportSettings.encodingQuality = TextureEncodingQuality::Default;
        if (ImGui::Selectable("Highest"))
            textureImportSettings.encodingQuality = TextureEncodingQuality::Highest;
        if (ImGui::Selectable("Fastest"))
            textureImportSettings.encodingQuality = TextureEncodingQuality::Fastest;
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Generate Mipmaps", &textureImportSettings.mips);
    ImGui::Checkbox("Linear", &textureImportSettings.linear);
    ImGui::Checkbox("Premultiply Alpha", &textureImportSettings.pma);

    ImGui::Separator();

    if (ImGui::Button("Apply")) {
        AssetRegistryEntry& entry = editorModule->GetProject()->GetAssetRegistry()->GetAsset(id);
        if (entry.type == Vin::AssetType::Texture)
            entry.metadata["Import Settings"] = textureImportSettings.Serialize();
        editorModule->ReimportAsset(id);
    }
}

Vin::String TextureInspector::GetTextureEncodingFormatString(TextureEncodingFormat format) {
    switch (format) {
        case TextureEncodingFormat::BC1:
            return "BC1";
        case TextureEncodingFormat::BC3:
            return "BC3";
        case TextureEncodingFormat::BC4:
            return "BC4";
        case TextureEncodingFormat::BC5:
            return "BC5";
    }
    return "None";
}

Vin::String TextureInspector::GetTextureEncodingQualityString(TextureEncodingQuality quality) {
    switch (quality) {
        case TextureEncodingQuality::Default:
            return "Default";
        case TextureEncodingQuality::Highest:
            return "Highest";
        case TextureEncodingQuality::Fastest:
            return "Fastest";
    }
    return "None";
}