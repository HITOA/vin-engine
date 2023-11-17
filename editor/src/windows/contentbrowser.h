#ifndef VIN_EDITOR_CONTENTBROWSER_H
#define VIN_EDITOR_CONTENTBROWSER_H

#include "../editor.h"

struct ContentEntry {
    char name[256]{};
    bool isDirectory{ false };
    Vin::AssetType type{};
};

class ContentBrowserWindow : public EditorWindow {
public:
    void Initialize() final;

    void Draw(bool* open) final;

    void Refresh(Vin::StringView path);

private:
    void DrawContentBrowserPopup();
    void AddFilesDialog();
    void SetInspectorToContentEntry(ContentEntry& entry);

private:
    Vin::StringView workingDir{};
    Vin::String currentDir{};
    Vin::Vector<ContentEntry> contents{};
    float zoom{ 64 };

    Vin::Ref<Vin::Texture> folderIcon{};
    Vin::Ref<Vin::Texture> imageIcon{};
    Vin::Ref<Vin::Texture> textIcon{};
    Vin::Ref<Vin::Texture> shaderIcon{};
};

#endif //VIN_EDITOR_CONTENTBROWSER_H
