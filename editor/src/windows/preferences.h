#ifndef VIN_ENGINE_PREFERENCES_H
#define VIN_ENGINE_PREFERENCES_H

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

private:
    PreferenceTab currentTab{};
};

#endif //VIN_ENGINE_PREFERENCES_H
