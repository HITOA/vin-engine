#include "inspectorasset.h"
#include <imgui.h>

void InspectorTextAsset::Draw() {

}


void InspectorTextureAsset::Draw() {
    ImGui::TextUnformatted(assetPath.c_str());

    if (ImGui::BeginCombo("Import Preset", "...")) {
        if (ImGui::Selectable("RGB Texture")) {
            importSettings = AssetTextureImportSettings{};
        }

        if (ImGui::Selectable("RGBA Texture")) {
            importSettings = AssetTextureImportSettings{};
            importSettings.pma = true;
            importSettings.encodingFormat = TextureEncodingFormat::BC3;
        }

        if (ImGui::Selectable("Normal Map")) {
            importSettings = AssetTextureImportSettings{};
            importSettings.linear = true;
            importSettings.encodingFormat = TextureEncodingFormat::BC4;
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();

    if (ImGui::BeginCombo("Encoding Format", GetTextureEncodingFormatString(importSettings.encodingFormat).c_str())) {
        if (ImGui::Selectable("BC1"))
            importSettings.encodingFormat = TextureEncodingFormat::BC1;
        if (ImGui::Selectable("BC3"))
            importSettings.encodingFormat = TextureEncodingFormat::BC3;
        if (ImGui::Selectable("BC4"))
            importSettings.encodingFormat = TextureEncodingFormat::BC4;
        if (ImGui::Selectable("BC5"))
            importSettings.encodingFormat = TextureEncodingFormat::BC5;
        ImGui::EndCombo();
    }

    if (ImGui::BeginCombo("Encoding Quality", GetTextureEncodingQualityString(importSettings.encodingQuality).c_str())) {
        if (ImGui::Selectable("Default"))
            importSettings.encodingQuality = TextureEncodingQuality::Default;
        if (ImGui::Selectable("Highest"))
            importSettings.encodingQuality = TextureEncodingQuality::Highest;
        if (ImGui::Selectable("Fastest"))
            importSettings.encodingQuality = TextureEncodingQuality::Fastest;
        ImGui::EndCombo();
    }

    ImGui::Checkbox("Generate Mipmaps", &importSettings.mips);
    ImGui::Checkbox("Linear", &importSettings.linear);
    ImGui::Checkbox("Premultiply Alpha", &importSettings.pma);

    ImGui::Separator();

    if (ImGui::Button("Apply")) {
        editor->AddTask([this](){
            std::filesystem::path path{ assetPath };
            editor->ImportTextureAsset(importSettings, path);
        }, "Reimporting Texture");
    }
}

Vin::String InspectorTextureAsset::GetTextureEncodingFormatString(TextureEncodingFormat format) {
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

Vin::String InspectorTextureAsset::GetTextureEncodingQualityString(TextureEncodingQuality quality) {
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