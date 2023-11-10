#ifndef VIN_EDITOR_PREFERENCES_H
#define VIN_EDITOR_PREFERENCES_H

#include "../editor.h"

enum class PreferenceTab {
    ProjectInfo,
    TextureImportSettings,
    ShaderImportSettings
};

class PreferencesWindow : public EditorWindow {
public:
    void Draw(bool* open) final;

    void DrawTextureImportSettingsTab();
    void DrawShaderImportSettingsTab();

private:
    PreferenceTab currentTab{};
};

#endif //VIN_EDITOR_PREFERENCES_H
