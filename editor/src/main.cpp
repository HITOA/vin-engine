#include <cxxopts.hpp>
#include <iostream>
#include "editor.h"
#include <nfd.h>
#include "msgbox.h"
#include <filesystem>

#include "windows/console.h"
#include "windows/contentbrowser.h"
#include "windows/preferences.h"

void RegisterEditorWindow(Vin::Ref<EditorModule> editor) {
    editor->RegisterEditorWindow(Vin::MakeRef<PreferencesWindow>(), "File/Preferences");

    editor->RegisterEditorWindow(Vin::MakeRef<ConsoleWindow>(), "Window/Console");
    editor->RegisterEditorWindow(Vin::MakeRef<ContentBrowserWindow>(), "Window/Content Browser");
}

int main(int argc, char** argv) {
    cxxopts::Options options{"Vin Editor", "Game Engine Editor for Vin Engine"};
    options.add_options()
            ("h,help", "Print usage.")
            ("d,dir", "Set the working directory of the editor.", cxxopts::value<std::string>())
            ("gapi", "Specify which graphics api to use for rendering in the editor (vk, gl, dx11, dx12)", cxxopts::value<std::string>());

    cxxopts::ParseResult result = options.parse(argc, argv);
    EditorOptions editorOptions{};

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!result.count("dir")) {
        nfdchar_t* workingDir = NULL;
        nfdresult_t r = NFD_PickFolder(NULL, &workingDir);
        if (r == NFD_OKAY) {
            editorOptions.workingDir = workingDir;
            free(workingDir);
        } else {
            free(workingDir);
            std::cout << "No working directory specified. Abort." << std::endl;
            Show("No working directory specified.", "Error", MessageType::Error, MessageButton::Quit);
            return 0;
        }
    } else {
        editorOptions.workingDir = result["dir"].as<std::string>();
    }

    if (!std::filesystem::is_directory(editorOptions.workingDir)) {
        std::cout << "Invalid working directory. Abort." << std::endl;
        Show("Invalid working directory.", "Error", MessageType::Error, MessageButton::Quit);
        return 0;
    }

    Vin::Modules::RenderingApi renderingApi{ Vin::Modules::RenderingApi::Count };

    if (result.count("gapi")) {
        std::string sapi = result["gapi"].as<std::string>();
        if (sapi == "vk")
            renderingApi = Vin::Modules::RenderingApi::Vulkan;
        else if (sapi == "gl")
            renderingApi = Vin::Modules::RenderingApi::OpenGL;
        else if (sapi == "dx11")
            renderingApi = Vin::Modules::RenderingApi::Direct3D11;
        else if (sapi == "dx12")
            renderingApi = Vin::Modules::RenderingApi::Direct3D12;
        else {
            std::cout << "Invalid graphic api specified : \"" << sapi.c_str() << "\"" << std::endl;
            return 0;
        }
    }

    Vin::Logger::Logger::AddLogOutput(&std::cout);

    Vin::App application{};

    application.AddModule<Vin::Modules::WindowModule>();
    application.AddModule<Vin::Modules::RenderingModule>(renderingApi);
    RegisterEditorWindow(application.AddModule<EditorModule>(editorOptions));


    application.Run();

}