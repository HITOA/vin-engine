cmake_minimum_required(VERSION 3.26)
project(vin_engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -lwayland-client -lwayland-egl")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(3rdparty/cxxopts)
add_subdirectory(3rdparty/imgui)

add_executable(vin_editor src/main.cpp
        src/editor.h
        src/editor.cpp
        src/big.h
        src/imgui/fsocornutimgui.h
        src/imgui/vsocornutimgui.h
        src/imgui/imgui_impl_glfw.h
        src/imgui/imgui_impl_glfw.cpp)
target_link_libraries(vin_editor vin_engine cxxopts imgui bx)