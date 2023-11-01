#ifndef VIN_ENGINE_CONTENTBROWSER_H
#define VIN_ENGINE_CONTENTBROWSER_H

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
    float zoom{96};
};

#endif //VIN_ENGINE_CONTENTBROWSER_H
