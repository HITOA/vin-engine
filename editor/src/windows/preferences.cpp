#include "preferences.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "../imgui/imguivin.h"
#include <nfd.h>

void PreferencesWindow::Draw(bool *open) {

    if (ImGui::Begin("Preferences", open)) {
        if (ImGui::Button("Apply")) {
            editor->SaveEditorConfig();
        }

        ImGui::Columns(2);
        if (ImGui::BeginChild("Tabs")) {
            ImGui::TextUnformatted("Project Info");
            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                currentTab = PreferenceTab::ProjectInfo;

            if (ImGui::TreeNode("Import Settings")) {
                ImGui::TextUnformatted("Texture");
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    currentTab = PreferenceTab::TextureImportSettings;

                ImGui::TextUnformatted("Shader");
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    currentTab = PreferenceTab::ShaderImportSettings;

                ImGui::TextUnformatted("General");
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
                    currentTab = PreferenceTab::GeneralImportSettings;

                ImGui::TreePop();
            }
        }
        ImGui::EndChild();

        ImGui::NextColumn();

        switch (currentTab) {
            case PreferenceTab::TextureImportSettings:
                DrawTextureImportSettingsTab();
                break;
            case PreferenceTab::ShaderImportSettings:
                DrawShaderImportSettingsTab();
                break;
            case PreferenceTab::GeneralImportSettings:
                DrawGeneralImportSettingsTab();
            default:
                break;
        }

        ImGui::Columns(1);
    }
    ImGui::End();

}

void PreferencesWindow::DrawTextureImportSettingsTab() {
    EditorImportSettings* importSettings = editor->GetEditorImportSettings();

    if (ImGui::BeginChild("Texture Import Settings")) {
        ImGui::TextUnformatted("Texture Compiler : ");
        ImGui::SameLine();
        ImGui::InputText("##TextureCompilerPath", &(importSettings->textureCompilerPath));
        ImGui::SameLine();
        if (ImGui::Button("...")) {
            nfdchar_t* path = nullptr;
            nfdresult_t  r = NFD_OpenDialog(NULL, NULL, &path);
            if (r == NFD_OKAY) {
                importSettings->textureCompilerPath = path;
                free(path);
            }
        }
    }
    ImGui::EndChild();
}

void PreferencesWindow::DrawShaderImportSettingsTab() {
    EditorImportSettings* importSettings = editor->GetEditorImportSettings();

    if (ImGui::BeginChild("Shader Import Settings")) {
        ImGui::TextUnformatted("Shader Compiler : ");
        ImGui::SameLine();
        ImGui::InputText("##ShaderCompilerPath", &(importSettings->shaderCompilerPath));
        ImGui::SameLine();
        if (ImGui::Button("...")) {
            nfdchar_t* path = nullptr;
            nfdresult_t  r = NFD_OpenDialog(NULL, NULL, &path);
            if (r == NFD_OKAY) {
                importSettings->shaderCompilerPath = path;
                free(path);
            }
        }
    }
    ImGui::EndChild();
}

void PreferencesWindow::DrawGeneralImportSettingsTab() {
    EditorImportSettings* importSettings = editor->GetEditorImportSettings();

    if (ImGui::BeginChild("General Import Settings")) {
        ImGui::TextUnformatted("Import new file : ");
        ImGui::SameLine();
        ImGui::Checkbox("##ImportNewFileInWorkingDir", &(importSettings->importNewFileInWorkingDir));
    }
    ImGui::EndChild();
}