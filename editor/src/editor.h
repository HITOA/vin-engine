#ifndef VIN_ENGINE_EDITOR_H
#define VIN_ENGINE_EDITOR_H

#include <vin/vin.h>
#include <vin/scene/resources/texture.h>
#include <vin/core/thread/threadpool.h>
#include <bgfx/bgfx.h>
#include "config/editorimportsettings.h"
#include "project.h"
#include "importer/assetimporter.h"

#define EDITOR_GUI_VIEW_ID 200

class EditorModule;

class EditorWindow {
public:
    virtual ~EditorWindow() = default;

    virtual void Initialize() {};

    virtual void Draw(bool* open) = 0;

public:
    EditorModule* editor{ nullptr };
};

struct EditorWindowEntry {
    Vin::Ref<EditorWindow> window{ nullptr, nullptr };
    bool open{ false };
    Vin::String path{};
    Vin::String name{};
};

struct EditorOptions {
    Vin::String workingDir{};
};

struct EditorTask {
    Vin::String name{ "Editor Task." };
    std::function<void()> job{};
    bool terminated{ false };
};

class EditorModule : public Vin::Module {
public:
    Vin::DependencyList<Vin::Modules::WindowModule, Vin::Modules::RenderingModule> dependencies{ windowModule, renderingModule };

    explicit EditorModule(EditorOptions& options);

    void Initialize() final;
    void Uninitialize() final;

    void Update(Vin::TimeStep) final;

    void RegisterEditorWindow(Vin::Ref<EditorWindow> window, Vin::StringView path);

    void SaveEditorConfig();
    void LoadEditorConfig();

    Vin::StringView GetWorkingDirectory();

    EditorImportSettings* GetEditorImportSettings();

    void ImportAsset(Vin::StringView path);
    void ImportTextAsset(AssetTextImportSettings& textImportSettings, std::filesystem::path& assetPath);
    void ImportTextureAsset(AssetTextureImportSettings& textureImportSettings, std::filesystem::path& assetPath);

    bool IsAssetImported(Vin::StringView rpath);
    Vin::Ref<Project> GetProject();

    void AddTask(std::function<void()> f, Vin::StringView name);

private:
    void DrawDockSpace();
    void DrawMainMenuBar();

    void InitImguiWithBgfx();
    void ShutdownImguiWithBgfx();

    void Begin();
    void End();

    const unsigned char* GetFSOcornut();
    const unsigned char* GetVSOcornut();
    unsigned int GetFSOcornutLength();
    unsigned int GetVSOcornutLength();

private:
    Vin::Ref<Vin::Modules::WindowModule> windowModule{};
    Vin::Ref<Vin::Modules::RenderingModule> renderingModule{};
    Vin::Vector<EditorWindowEntry> editorWindows{};

    bgfx::VertexLayout  imguiVertexLayout{};
    bgfx::TextureHandle imguiFontTexture{};
    bgfx::UniformHandle imguiFontUniform{};
    bgfx::ProgramHandle imguiProgram{};

    Vin::Ref<Project> project{ nullptr, nullptr };

    EditorOptions options{};
    EditorImportSettings importSettings{};

    Vin::Core::ThreadPool threadPool{};
    Vin::Queue<EditorTask> editorTasks{};
    std::mutex m{};
};

#endif //VIN_ENGINE_EDITOR_H
