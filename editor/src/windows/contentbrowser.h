#ifndef VIN_ENGINE_CONTENTBROWSER_H
#define VIN_ENGINE_CONTENTBROWSER_H

#include "../editor.h"

struct ContentEntry {
    char name[256]{ false };
    bool isDirectory{ false };
};

class ContentBrowserWindow : public EditorWindow {
public:
    void Initialize() final;

    void Draw(bool* open) final;

    void Refresh(Vin::StringView path);

private:
    void DrawContentBrowserPopup();
    void DrawContentBrowserEntryPopup();
    void AddFilesDialog();

private:
    Vin::StringView workingDir{};
    Vin::String currentDir{};
    Vin::Vector<ContentEntry> contents{};
    float zoom{96};
};

#endif //VIN_ENGINE_CONTENTBROWSER_H
