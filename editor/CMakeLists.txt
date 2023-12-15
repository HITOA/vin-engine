cmake_minimum_required(VERSION 3.26)
project(vin_editor)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    set(CMAKE_CXX_FLAGS "/W4")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (VIN_BUILD_WITH_WAYLAND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lwayland-client -lwayland-egl")
endif()

add_subdirectory(3rdparty/cxxopts)
add_subdirectory(3rdparty/imgui)
add_subdirectory(3rdparty/nativefiledialog)
add_subdirectory(3rdparty/efsw)
add_subdirectory(3rdparty/assimp)

add_executable(vin_editor src/main.cpp
        src/editor.h
        src/editor.cpp
        src/imgui/fsocornutimgui.h
        src/imgui/vsocornutimgui.h
        src/imgui/imgui_impl_glfw.h
        src/imgui/imgui_impl_glfw.cpp
        src/windows/console.h
        src/windows/console.cpp
        src/msgbox.h
        src/windows/preferences.h
        src/windows/preferences.cpp
        src/windows/contentbrowser.h
        src/windows/contentbrowser.cpp
        src/config/editorimportsettings.h
        src/config/editorconfig.h
        src/config/editorconfig.cpp
        src/config/editorimportsettings.cpp
        src/imgui/imguivin.h
        src/imgui/imguivin.cpp
        src/exec.h
        src/importer/assetimporter.h
        src/importer/assettext.h
        src/windows/inspector.h
        src/inspector/inspector.h
        src/windows/inspector.cpp
        src/editorfilewatcher.h
        src/editorfilewatcher.cpp
        src/importer/assetmesh.h
        src/project/assetregistry.h
        src/project/project.h
        src/importer/textureimporter.h
        src/importer/textureimporter.cpp
        src/project/assetregistry.cpp
        src/project/project.cpp
        src/importer/assetimportermanager.h
        src/importer/assetimportermanager.cpp
        src/inspector/textureinspector.h
        src/inspector/textureinspector.cpp
        src/importer/shaderimporter.h
        src/importer/shaderimporter.cpp
        src/importer/meshimporter.h
        src/importer/meshimporter.cpp
        src/importer/programimporter.h
        src/importer/programimporter.cpp
        src/importer/materialimporter.h
        src/importer/materialimporter.cpp
        ../tools/shadercompiler/main.cpp
)
target_link_libraries(vin_editor vin_engine cxxopts imgui bx nativefiledialog efsw-static assimp)