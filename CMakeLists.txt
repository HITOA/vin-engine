cmake_minimum_required(VERSION 3.2)
project(vin)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
set(GLAD_SOURCES_DIR ${EXTERNAL_PATH}/glad)

add_subdirectory(${EXTERNAL_PATH}/glad/cmake)
add_subdirectory(${EXTERNAL_PATH}/glfw)
add_subdirectory(${EXTERNAL_PATH}/googletest)
add_subdirectory(${EXTERNAL_PATH}/Lua)
add_subdirectory(${EXTERNAL_PATH}/physfs)

glad_add_library(glad_library STATIC API vulkan=1.2)

add_library(vin_engine STATIC 
	include/core.hpp
	include/vin.hpp
	include/window.hpp

	include/events/event.hpp
	include/events/application_event.hpp
	include/events/input_event.hpp

	src/entrypoint.cpp
	src/event.cpp
	src/vin.cpp

	src/platform/windows/window_windows.hpp
	src/platform/windows/window_windows.cpp
)

target_include_directories(vin_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(
	vin_engine 
	glad_library 
	glfw 
	lua_static 
	physfs-static 
	GTest::gtest_main
)

target_precompile_headers(vin_engine PUBLIC include/vinpch.hpp)

file(COPY ${CMAKE_SOURCE_DIR}/bin/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(vin_test 
	test/main.cpp 
	test/test.hpp
)

target_link_libraries(
  vin_test
  vin_engine
  GTest::gtest_main
)