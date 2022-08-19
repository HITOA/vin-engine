cmake_minimum_required(VERSION 3.2)
project(vin)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bin
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DVIN_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DVIN_DEBUG")

set(CMAKE_CXX_STANDARD 11)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
set(GLAD_SOURCES_DIR ${EXTERNAL_PATH}/glad)

add_subdirectory(${EXTERNAL_PATH}/EASTL)
add_subdirectory(${EXTERNAL_PATH}/fmt)
add_subdirectory(${EXTERNAL_PATH}/glad/cmake)
add_subdirectory(${EXTERNAL_PATH}/glfw)
add_subdirectory(${EXTERNAL_PATH}/googletest)
add_subdirectory(${EXTERNAL_PATH}/Lua)
add_subdirectory(${EXTERNAL_PATH}/physfs)

glad_add_library(glad_library STATIC API gl:core=4.6)

include_directories(${EXTERNAL_PATH}/stb)
include_directories(${EXTERNAL_PATH}/vin-base/src)

file(GLOB_RECURSE vin_engine_src "src/*.cpp" "src/*.hpp")

add_library(vin_engine STATIC ${vin_engine_src})

target_link_libraries(
	vin_engine 
	glad_library 
	glfw 
	EASTL
	lua_static 
	physfs-static 
	GTest::gtest_main
	fmt
)

target_precompile_headers(vin_engine PUBLIC src/vinpch.hpp)
target_include_directories(vin_engine PUBLIC src)

add_executable(vin_test 
	test/main.cpp
)

target_link_libraries(
  vin_test
  vin_engine
  GTest::gtest_main
)