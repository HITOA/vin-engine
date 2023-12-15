cmake_minimum_required(VERSION 3.26)
project(vin_engine_shader_compiler)

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

set(ENABLE_OPT OFF)
add_subdirectory(glslang)

add_executable(vin_shader_compiler main.cpp)
target_link_libraries(vin_shader_compiler glslang glslang-default-resource-limits SPIRV )