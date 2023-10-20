#include <cxxopts.hpp>
#include <iostream>
#include "editor.h"
#include "windows/console.h"

void RegisterEditorWindow(Vin::Ref<EditorModule> editor) {
    editor->RegisterEditorWindow(Vin::MakeRef<Console>(), "Window/Console");
}

int main(int argc, char** argv) {
    cxxopts::Options options{"Vin Editor", "Game Engine Editor for Vin Engine"};
    options.add_options()
            ("h,help", "Print usage.")
            ("d,dir", "Set the working directory of the editor.", cxxopts::value<std::string>())
            ("gapi", "Specify which graphics api to use for rendering in the editor (vk, gl, dx11, dx12)", cxxopts::value<std::string>());

    cxxopts::ParseResult result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    /*if (!result.count("dir")) {
        std::cout << "No working directory specified" << std::endl;
        return 0;
    }*/

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
    RegisterEditorWindow(application.AddModule<EditorModule>());


    application.Run();

}