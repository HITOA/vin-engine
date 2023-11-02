#include "editor.h"
#include <imgui.h>
#include "imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/fsocornutimgui.h"
#include "imgui/vsocornutimgui.h"
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <filesystem>
#include "msgbox.h"
#include <fstream>

EditorModule::EditorModule(EditorOptions& options) : options{ options } {
    std::filesystem::path workingDir{ options.workingDir };
    if (!std::filesystem::exists(workingDir / "project.vin")) {
        MessageResult r = Show("Project file doesn't exists in working directory, do you want to create one ?",
                               "Project file doesn't exists.", MessageType::Question, MessageButton::YesNo);
        switch (r) {
            case MessageResult::Yes:
                project = Vin::MakeRef<Project>(PATH_TO_STRING(std::filesystem::path{ workingDir / "project.vin" }));
                break;
            case MessageResult::No:
                exit(0);
                break;
            default:
                break;
        }
    } else {
        project = Vin::MakeRef<Project>(PATH_TO_STRING(std::filesystem::path{ workingDir / "project.vin" }));
    }
}

void EditorModule::Initialize() {
    LoadEditorConfig();

    if (!windowModule) {
        Vin::Logger::Err("A Window module is needed for the editor to function.");
        abort();
    }

    if (!renderingModule) {
        Vin::Logger::Err("A Rendering module is needed for the editor to function.");
        abort();
    }

    ImGui::CreateContext();
    InitImguiWithBgfx();
    ImGui_ImplGlfw_InitForOther(windowModule->GetGlfwWindow(), true);

    for (auto& window : editorWindows)
        window.window->Initialize();
}

void EditorModule::Uninitialize() {
    ShutdownImguiWithBgfx();
    ImGui::DestroyContext();
}

void EditorModule::Update(Vin::TimeStep dt) {
    Begin();

    DrawDockSpace();
    DrawMainMenuBar();

    for (auto& entry : editorWindows)
        if (entry.open)
            entry.window->Draw(&entry.open);

    End();
}

void EditorModule::RegisterEditorWindow(Vin::Ref<EditorWindow> window, Vin::StringView path) {
    EditorWindowEntry entry{};
    entry.window = window;
    entry.open = false;
    entry.path = path;

    auto it = std::find(path.crbegin(), path.crend(), '/');
    if (it == std::rend(path))
        return;

    entry.name = path.substr(std::distance(std::rbegin(path), it));
    entry.window->editor = this;
    editorWindows.push_back(entry);
}

void EditorModule::SaveEditorConfig() {
    Config config{};

    importSettings.Serialize(config);

    Vin::String serializedConfig = SerializeConfig(config);

    Vin::String configPath = GetConfigDir() + "/editor.ini";
    std::ofstream configFile{ std::filesystem::path{ configPath } };
    configFile << serializedConfig;
    configFile.close();
}

void EditorModule::LoadEditorConfig() {
    Vin::String configPath = GetConfigDir() + "/editor.ini";

    if (!std::filesystem::exists(configPath))
        return;

    std::ifstream configFile{ std::filesystem::path{ configPath } };
    configFile.ignore(std::numeric_limits<std::streamsize>::max());
    std::size_t length = configFile.gcount();
    configFile.clear();
    configFile.seekg(0, std::ios_base::beg);
    Vin::String serializedConfig{};
    serializedConfig.resize(length);
    configFile.read(serializedConfig.data(), length);

    Config config = DeserializeConfig(serializedConfig);

    importSettings.Deserialize(config);
}

Vin::StringView EditorModule::GetWorkingDirectory() {
    return options.workingDir;
}

EditorImportSettings* EditorModule::GetEditorImportSettings() {
    return &importSettings;
}

void EditorModule::ImportAsset(Vin::StringView path) {
    std::filesystem::path assetPath{ path };

    Vin::AssetType type = GetType(PATH_TO_STRING(assetPath.extension()));

    if (type == Vin::AssetType::None) {
        Vin::Logger::Err("Can't import \"", path, "\" : Unknown asset type.");
        return;
    }

    switch (type) {
        case Vin::AssetType::Text:
            {
                AssetTextImportSettings textImportSettings{};
                ImportTextAsset(textImportSettings, assetPath);
                break;
            }
        case Vin::AssetType::Texture:
            {
                AssetTextureImportSettings textureImportSettings{};
                ImportTextureAsset(textureImportSettings, assetPath);
                break;
            }
        default:
            break;
    }
}

void EditorModule::ImportTextAsset(AssetTextImportSettings &textImportSettings, std::filesystem::path &assetPath) {
    std::filesystem::path relPath = std::filesystem::relative(assetPath, options.workingDir);
    AssetImporter<Vin::AssetType::Text> importer{};
    Vin::String importedPath = importer(PATH_TO_STRING(assetPath), importSettings, textImportSettings);
    project->ImportTextAsset(PATH_TO_STRING(relPath), PATH_TO_STRING(std::filesystem::relative(importedPath, options.workingDir)), textImportSettings);
}

void EditorModule::ImportTextureAsset(AssetTextureImportSettings &textureImportSettings, std::filesystem::path &assetPath) {
    std::filesystem::path relPath = std::filesystem::relative(assetPath, options.workingDir);
    AssetImporter<Vin::AssetType::Texture> importer{};
    Vin::String importedPath = importer(PATH_TO_STRING(assetPath), importSettings, textureImportSettings);
    project->ImportTextureAsset(PATH_TO_STRING(relPath), PATH_TO_STRING(std::filesystem::relative(importedPath, options.workingDir)), textureImportSettings);
}

bool EditorModule::IsAssetImported(Vin::StringView rpath) {
    return project->IsAssetImported(rpath);
}

Vin::Ref<Project> EditorModule::GetProject() {
    return project;
}

void EditorModule::DrawDockSpace() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("Main DockSpace", nullptr,
                 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
                 ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoBackground);

    ImGui::PopStyleVar(3);

    ImGui::DockSpace(ImGui::GetID("Main DockSpace"), ImVec2{ 0, 0 }, ImGuiDockNodeFlags_PassthruCentralNode);

    ImGui::End();
}

void EditorModule::DrawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {

            ImGui::EndMenu();
        }

        for (auto& entry : editorWindows) {

            Vin::String curr = entry.path;
            auto it = std::find(curr.begin(), curr.end(), '/');
            size_t i = 0;
            bool open = true;

            while (it != curr.end()) {
                size_t n = std::distance(curr.begin(), it);
                //Vin::Logger::Log(curr.substr(0, n));
                if (!ImGui::BeginMenu(curr.substr(0, n).c_str())) {
                    open = false;
                    break;
                }
                curr = curr.substr(n + 1);
                it = std::find(curr.begin(), curr.end(), '/');
                ++i;
            }

            if (open) {
                if (ImGui::MenuItem(curr.c_str())) {
                    entry.open = true;
                }
            }

            for (;i > 0; --i) {
                ImGui::EndMenu();
            }

        }

        ImGui::EndMainMenuBar();
    }
}

void EditorModule::InitImguiWithBgfx() {
    unsigned char* data;
    int width, height;

    windowModule->onWindowResize += [](int width, int height) {
        bgfx::setViewRect( EDITOR_GUI_VIEW_ID, 0, 0, width, height );
    };

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    imguiVertexLayout.begin()
            .begin()
            .add( bgfx::Attrib::Position, 2, bgfx::AttribType::Float )
            .add( bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float )
            .add( bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true )
            .end();

    io.Fonts->AddFontDefault();
    io.Fonts->GetTexDataAsRGBA32( &data, &width, &height );
    imguiFontTexture = bgfx::createTexture2D( ( uint16_t )width, ( uint16_t )height, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx::copy( data, width*height * 4 ) );
    imguiFontUniform = bgfx::createUniform( "s_tex", bgfx::UniformType::Sampler );

    bgfx::ShaderHandle vs = bgfx::createShader( bgfx::makeRef( GetVSOcornut(), GetVSOcornutLength() ) );
    bgfx::ShaderHandle fs = bgfx::createShader( bgfx::makeRef( GetFSOcornut(), GetFSOcornutLength() ) );
    imguiProgram = bgfx::createProgram( vs, fs, true );
}

void EditorModule::ShutdownImguiWithBgfx() {
    bgfx::destroy(imguiFontTexture);
    bgfx::destroy(imguiFontTexture);
    bgfx::destroy(imguiProgram);
}

void EditorModule::Begin() {
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void EditorModule::End() {
    ImGui::Render();
    ImDrawData* drawData = ImGui::GetDrawData();

    bgfx::setViewName(EDITOR_GUI_VIEW_ID, "ImGui");
    bgfx::setViewMode(EDITOR_GUI_VIEW_ID, bgfx::ViewMode::Sequential);

    const bgfx::Caps* caps = bgfx::getCaps();
    {
        float ortho[16];
        float x = drawData->DisplayPos.x;
        float y = drawData->DisplayPos.y;
        float width = drawData->DisplaySize.x;
        float height = drawData->DisplaySize.y;

        bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(EDITOR_GUI_VIEW_ID, NULL, ortho);
        bgfx::setViewRect(EDITOR_GUI_VIEW_ID, 0, 0, uint16_t(width), uint16_t(height) );
    }

    for ( int ii = 0, num = drawData->CmdListsCount; ii < num; ++ii )
    {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;

        const ImDrawList* drawList = drawData->CmdLists[ ii ];
        uint32_t numVertices = ( uint32_t )drawList->VtxBuffer.size();
        uint32_t numIndices  = ( uint32_t )drawList->IdxBuffer.size();

        if ( !bgfx::getAvailTransientVertexBuffer( numVertices, imguiVertexLayout ) || !bgfx::getAvailTransientIndexBuffer( numIndices ) )
        {
            break;
        }

        bgfx::allocTransientVertexBuffer( &tvb, numVertices, imguiVertexLayout );
        bgfx::allocTransientIndexBuffer( &tib, numIndices );

        ImDrawVert* verts = ( ImDrawVert* )tvb.data;
        memcpy( verts, drawList->VtxBuffer.begin(), numVertices * sizeof( ImDrawVert ) );

        ImDrawIdx* indices = ( ImDrawIdx* )tib.data;
        memcpy( indices, drawList->IdxBuffer.begin(), numIndices * sizeof( ImDrawIdx ) );

        uint32_t offset = 0;
        for ( const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd )
        {
            if ( cmd->UserCallback )
            {
                cmd->UserCallback( drawList, cmd );
            }
            else if ( 0 != cmd->ElemCount )
            {
                uint64_t state = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;
                bgfx::TextureHandle th = imguiFontTexture;
                if ( cmd->TextureId != NULL )
                {
                    th.idx = uint16_t( uintptr_t( cmd->TextureId ) );
                }
                state |= BGFX_STATE_BLEND_FUNC( BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA );
                const uint16_t xx = uint16_t( bx::max( cmd->ClipRect.x, 0.0f ) );
                const uint16_t yy = uint16_t( bx::max( cmd->ClipRect.y, 0.0f ) );
                bgfx::setScissor( xx, yy, uint16_t( bx::min( cmd->ClipRect.z, 65535.0f ) - xx ), uint16_t( bx::min( cmd->ClipRect.w, 65535.0f ) - yy ) );
                bgfx::setState( state );
                bgfx::setTexture( 0, imguiFontUniform, th );
                bgfx::setVertexBuffer( 0, &tvb, 0, numVertices );
                bgfx::setIndexBuffer( &tib, offset, cmd->ElemCount );
                bgfx::submit( EDITOR_GUI_VIEW_ID, imguiProgram );
            }

            offset += cmd->ElemCount;
        }
    }
}

const unsigned char* EditorModule::GetFSOcornut() {
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop: return nullptr;
		case bgfx::RendererType::Direct3D9:  return fs_ocornut_imgui_dx9;
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12: return fs_ocornut_imgui_dx11;
		case bgfx::RendererType::OpenGL:     return fs_ocornut_imgui_glsl;
		case bgfx::RendererType::OpenGLES:   return fs_ocornut_imgui_essl;
		case bgfx::RendererType::Gnm:        return nullptr;
		case bgfx::RendererType::Metal:      return fs_ocornut_imgui_mtl;
		case bgfx::RendererType::Vulkan:     return fs_ocornut_imgui_spv;
		case bgfx::RendererType::Count:      return nullptr;
    }
    return nullptr;
}

const unsigned char* EditorModule::GetVSOcornut() {
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop: return nullptr;
        case bgfx::RendererType::Direct3D9:  return vs_ocornut_imgui_dx9;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: return vs_ocornut_imgui_dx11;
        case bgfx::RendererType::OpenGL:     return vs_ocornut_imgui_glsl;
        case bgfx::RendererType::OpenGLES:   return vs_ocornut_imgui_essl;
        case bgfx::RendererType::Gnm:        return nullptr;
        case bgfx::RendererType::Metal:      return vs_ocornut_imgui_mtl;
        case bgfx::RendererType::Vulkan:     return vs_ocornut_imgui_spv;
        case bgfx::RendererType::Count:      return nullptr;
    }
    return nullptr;
}

unsigned int EditorModule::GetFSOcornutLength() {
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop: return 0;
        case bgfx::RendererType::Direct3D9:  return sizeof(fs_ocornut_imgui_dx9);
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: return sizeof(fs_ocornut_imgui_dx11);
        case bgfx::RendererType::OpenGL:     return sizeof(fs_ocornut_imgui_glsl);
        case bgfx::RendererType::OpenGLES:   return sizeof(fs_ocornut_imgui_essl);
        case bgfx::RendererType::Gnm:        return 0;
        case bgfx::RendererType::Metal:      return sizeof(fs_ocornut_imgui_mtl);
        case bgfx::RendererType::Vulkan:     return sizeof(fs_ocornut_imgui_spv);
        case bgfx::RendererType::Count:      return 0;
    }
    return 0;
}

unsigned int EditorModule::GetVSOcornutLength() {
    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop: return 0;
        case bgfx::RendererType::Direct3D9:  return sizeof(vs_ocornut_imgui_dx9);
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12: return sizeof(vs_ocornut_imgui_dx11);
        case bgfx::RendererType::OpenGL:     return sizeof(vs_ocornut_imgui_glsl);
        case bgfx::RendererType::OpenGLES:   return sizeof(vs_ocornut_imgui_essl);
        case bgfx::RendererType::Gnm:        return 0;
        case bgfx::RendererType::Metal:      return sizeof(vs_ocornut_imgui_mtl);
        case bgfx::RendererType::Vulkan:     return sizeof(vs_ocornut_imgui_spv);
        case bgfx::RendererType::Count:      return 0;
    }
    return 0;
}