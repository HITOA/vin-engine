cmake_minimum_required(VERSION 3.2)
cmake_policy(SET CMP0079 NEW)
project(vin)

function(make_includable input_file output_file var_name)
    file(READ ${input_file} content)
    set(delim "RESOURCE_DELIM")
    set(content "const char* ${var_name} = R\"${delim}(\n${content})${delim}\";")
    file(WRITE ${output_file} "${content}")
endfunction(make_includable)

make_includable(src/resources/shaderlibrary/vin.glsl generated/vinshader.hpp vinshader)
make_includable(src/resources/shaderlibrary/vininput.glsl generated/vininputshader.hpp vininputshader)
make_includable(src/resources/shaderlibrary/vinrealtimelights.glsl generated/vinrealtimelightsshader.hpp vinrealtimelightsshader)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
    add_compile_options(/W4 /EHsc)
else()
    add_compile_options(-W4 -pedantic -fexceptions -fcxx-exceptions)
endif()

set(CMAKE_CXX_FLAGS "-W4")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVIN_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DVIN_DEBUG")

set(CMAKE_CXX_STANDARD 17)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
set(GLAD_SOURCES_DIR ${EXTERNAL_PATH}/glad)
set(LZ4_BUNDLED_MODE ON)

add_subdirectory(${EXTERNAL_PATH}/fmt)
add_subdirectory(${EXTERNAL_PATH}/glad/cmake)
add_subdirectory(${EXTERNAL_PATH}/glfw)
add_subdirectory(${EXTERNAL_PATH}/imgui)
add_subdirectory(${EXTERNAL_PATH}/lz4/build/cmake)
add_subdirectory(${EXTERNAL_PATH}/mimalloc)
add_subdirectory(${EXTERNAL_PATH}/optick-compiler)
add_subdirectory(${EXTERNAL_PATH}/tinygltf)

glad_add_library(glad_library STATIC API gl:core=4.6)

include_directories(${EXTERNAL_PATH}/stb)
include_directories(${EXTERNAL_PATH}/tinyobjloader)

file(GLOB_RECURSE vin_engine_src "src/*.cpp" "src/*.hpp" "src/*.h" "generated/*.hpp")

add_library(vin_engine STATIC ${vin_engine_src})

target_link_libraries(
	vin_engine 
	glad_library 
	glfw 
	imgui
	lz4_static
	mimalloc-static
	fmt
	tinygltf
	OptickCore
)

target_precompile_headers(vin_engine PUBLIC src/vinpch.hpp)
target_include_directories(vin_engine PUBLIC src)
target_include_directories(vin_engine PUBLIC generated)

add_executable(vin_test 
	test/main.cpp
)

target_link_libraries(
  vin_test
  vin_engine
)