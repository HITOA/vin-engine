#include "contentbrowser.h"
#include "inspector.h"
#include <filesystem>
#include <imgui.h>
#include <nfd.h>
#include "../inspector/textureinspector.h"

void ContentBrowserWindow::Initialize() {
    folderIcon = Vin::ResourceManager::Load<Vin::Texture>("/icons/default-folder.vasset");
    imageIcon = Vin::ResourceManager::Load<Vin::Texture>("/icons/image-x-generic.vasset");
    textIcon = Vin::ResourceManager::Load<Vin::Texture>("/icons/text-x-generic.vasset");
    shaderIcon = Vin::ResourceManager::Load<Vin::Texture>("/icons/text-x-shader.vasset");

    workingDir = editor->GetProject()->GetWorkingDirectory();
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

        ImGui::SliderFloat("Zoom", &zoom, 32, 128);

        float width = ImGui::GetContentRegionAvail().x;
        int cCount = (int)(width / (zoom + 16.0f));

        ImGui::Columns(cCount >= 1 ? cCount : 1, nullptr, false);

        size_t i = 0;
        for (auto& entry : contents) {
            if (entry.isDirectory) {
                ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
                ImGui::ImageButton(entry.name, (ImTextureID)folderIcon->GetTextureHandle().idx, { zoom, zoom }, { 0, 0 }, { 1, 1 });
                ImGui::PopStyleColor();
            } else {
                switch (entry.type) {
                    case Vin::AssetType::Text:
                        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
                        ImGui::ImageButton(entry.name, (ImTextureID)textIcon->GetTextureHandle().idx, { zoom, zoom }, { 0, 0 }, { 1, 1 });
                        ImGui::PopStyleColor();
                        break;
                    case Vin::AssetType::Texture:
                        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
                        ImGui::ImageButton(entry.name, (ImTextureID)imageIcon->GetTextureHandle().idx, { zoom, zoom }, { 0, 0 }, { 1, 1 });
                        ImGui::PopStyleColor();
                        break;
                    case Vin::AssetType::Shader:
                        ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
                        ImGui::ImageButton(entry.name, (ImTextureID)shaderIcon->GetTextureHandle().idx, { zoom, zoom }, { 0, 0 }, { 1, 1 });
                        ImGui::PopStyleColor();
                        break;
                    default:
                        ImGui::Button(entry.name, { zoom, zoom });
                        break;
                }
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                if (entry.isDirectory) {
                    Refresh(currentDir + entry.name + "/");
                }
            }
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                //TODO?
            }
            if (ImGui::IsItemFocused() && !entry.isDirectory) {
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
            strcpy(contentEntry.name, PATH_TO_STRING(entry.path().filename()).c_str());
            contentEntry.isDirectory = true;
            contents.push_back(contentEntry);
        } else {
            std::filesystem::path curr{ std::filesystem::relative(std::filesystem::weakly_canonical(entry), workingDir) };
            AssetRegistryEntry& entry = editor->GetProject()->GetAssetRegistry()->GetAsset(PATH_TO_STRING(curr));
            if (entry.type == Vin::AssetType::None)
                continue;

            ContentEntry contentEntry{};
            strcpy(contentEntry.name, PATH_TO_STRING(curr.filename()).c_str());
            contentEntry.isDirectory = false;
            contentEntry.type = entry.type;
            contentEntry.id = PATH_TO_STRING(curr);
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
    nfdresult_t r = NFD_OpenDialogMultiple(NULL, PATH_TO_STRING(std::filesystem::weakly_canonical(dir)).c_str(), &pathSet);
    if (r == NFD_OKAY) {
        for (size_t i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i) {
            std::filesystem::path currentPath{ NFD_PathSet_GetPath(&pathSet, i) };
            currentPath = std::filesystem::weakly_canonical(std::filesystem::relative(currentPath, workingDir));
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

    rpath = std::filesystem::weakly_canonical(rpath);
    apath = std::filesystem::weakly_canonical(apath);

    switch (entry.type) {
        case Vin::AssetType::Text: {
            /*AssetTextImportSettings importSettings = editor->GetProject()->GetTextAssetImportSettings(PATH_TO_STRING(rpath));
            Vin::Ref<InspectorTextAsset> textAssetInspector{ Vin::MakeRef<InspectorTextAsset>() };
            textAssetInspector->importSettings = importSettings;
            textAssetInspector->assetPath = PATH_TO_STRING(apath);
            textAssetInspector->editor = editor;
            InspectorWindow::SetInspector(textAssetInspector);*/
            break;
        }
        case Vin::AssetType::Texture: {
            Vin::Ref<TextureInspector> inspector = Vin::MakeRef<TextureInspector>(entry.id, editor);
            InspectorWindow::SetInspector(inspector);
            break;
        }
        default:
            break;
    }
}