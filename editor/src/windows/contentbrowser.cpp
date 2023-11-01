#include "contentbrowser.h"
#include "inspector.h"
#include "../inspector/inspectorasset.h"
#include <filesystem>
#include <imgui.h>
#include <nfd.h>

void ContentBrowserWindow::Initialize() {
    workingDir = editor->GetWorkingDirectory();
    Refresh("");
}

void ContentBrowserWindow::Draw(bool *open) {
    if (ImGui::Begin("Content Browser", open)) {

        const bool openPopup = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootWindow) && ImGui::IsMouseClicked(ImGuiMouseButton_Right);
        if (!ImGui::IsAnyItemHovered() && openPopup) {
            ImGui::OpenPopup("ContentBrowserPopup");
        }

        DrawContentBrowserPopup();

        if (ImGui::Button("Refresh")) {
            Refresh(currentDir);
        }

        ImGui::SameLine();

        ImGui::SliderFloat("Zoom", &zoom, 32, 256);

        float width = ImGui::GetContentRegionAvail().x;
        int cCount = (int)(width / (zoom + 16.0f));

        ImGui::Columns(cCount >= 1 ? cCount : 1, nullptr, false);

        size_t i = 0;
        for (auto& entry : contents) {
            ImGui::Button(entry.name, { zoom, zoom });
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (entry.isDirectory) {
                    Refresh(currentDir + entry.name + "/");
                }
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                //TODO?
            }
            if (ImGui::IsItemFocused()) {
                SetInspectorToContentEntry(entry);
            }

            ImGui::TextWrapped(entry.name);

            ImGui::NextColumn();
            ++i;
        }

        ImGui::Columns();
    }
    ImGui::End();
}

void ContentBrowserWindow::Refresh(Vin::StringView path) {
    std::filesystem::path dir{ workingDir };
    dir /= path;
    contents.clear();
    for (auto const& entry : std::filesystem::directory_iterator{ dir }) {
        if (entry.is_directory()) {
            ContentEntry contentEntry{};
            strcpy(contentEntry.name, entry.path().filename().c_str());
            contentEntry.isDirectory = true;
            contents.push_back(contentEntry);
        } else {
            std::filesystem::path curr{ std::filesystem::relative(entry, workingDir) };
            if (!editor->IsAssetImported(curr.c_str()))
                continue;

            ContentEntry contentEntry{};
            strcpy(contentEntry.name, entry.path().filename().c_str());
            contentEntry.isDirectory = false;
            contentEntry.type = GetType(entry.path().extension().c_str());
            contents.push_back(contentEntry);
        }
    }
    currentDir = path;
}

void ContentBrowserWindow::DrawContentBrowserPopup() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
    if (ImGui::BeginPopup("ContentBrowserPopup")) {

        if (ImGui::Button("Add File(s)")) {
            AddFilesDialog();
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::Button("Refresh")) {
            Refresh(currentDir);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
}

void ContentBrowserWindow::AddFilesDialog() {
    std::filesystem::path dir{ workingDir };
    dir /= currentDir;
    nfdpathset_t pathSet{};
    nfdresult_t r = NFD_OpenDialogMultiple(NULL, dir.c_str(), &pathSet);
    if (r == NFD_OKAY) {
        for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i) {
            std::filesystem::path currentPath{ NFD_PathSet_GetPath(&pathSet, i) };
            currentPath = std::filesystem::relative(currentPath, workingDir);
            if (memcmp(currentPath.c_str(), "../", 3) == 0) {
                Vin::Logger::Err("File \"", (const char*)NFD_PathSet_GetPath(&pathSet, i), "\" not in the working directory. ");
                continue;
            }
            editor->ImportAsset(NFD_PathSet_GetPath(&pathSet, i));
        }
        NFD_PathSet_Free(&pathSet);
    }

    Refresh(currentDir);
}

void ContentBrowserWindow::SetInspectorToContentEntry(ContentEntry &entry) {
    std::filesystem::path rpath{ currentDir };
    rpath /= entry.name;
    std::filesystem::path apath{ workingDir };
    apath /= rpath;

    switch (entry.type) {
        case Vin::AssetType::Text: {
            AssetTextImportSettings importSettings = editor->GetProject()->GetTextAssetImportSettings(rpath.c_str());
            Vin::Ref<InspectorTextAsset> textAssetInspector{ Vin::MakeRef<InspectorTextAsset>() };
            textAssetInspector->importSettings = importSettings;
            textAssetInspector->assetPath = apath.c_str();
            textAssetInspector->editor = editor;
            InspectorWindow::SetInspector(textAssetInspector);
            break;
        }
        case Vin::AssetType::Texture: {
            AssetTextureImportSettings importSettings = editor->GetProject()->GetTextureAssetImportSettings(rpath.c_str());
            Vin::Ref<InspectorTextureAsset> textAssetInspector{ Vin::MakeRef<InspectorTextureAsset>() };
            textAssetInspector->importSettings = importSettings;
            textAssetInspector->assetPath = apath.c_str();
            textAssetInspector->editor = editor;
            InspectorWindow::SetInspector(textAssetInspector);
            break;
        }
        default:
            break;
    }
}