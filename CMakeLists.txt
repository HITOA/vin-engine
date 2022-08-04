cmake_minimum_required(VERSION 3.2)
project(vin)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DVIN_DEBUG")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
set(GLAD_SOURCES_DIR ${EXTERNAL_PATH}/glad)

add_subdirectory(${EXTERNAL_PATH}/fmt)
add_subdirectory(${EXTERNAL_PATH}/glad/cmake)
add_subdirectory(${EXTERNAL_PATH}/glfw)
add_subdirectory(${EXTERNAL_PATH}/googletest)
add_subdirectory(${EXTERNAL_PATH}/Lua)
add_subdirectory(${EXTERNAL_PATH}/physfs)

glad_add_library(glad_library STATIC API gl:core=4.6)

add_library(vin_engine STATIC 
	src/core/application.cpp
	src/core/application.hpp
	src/core/assert.hpp
	src/core/entrypoint.cpp
	src/core/logger.cpp
	src/core/logger.hpp
	src/core/module.cpp
	src/core/module.hpp
	src/core/task.hpp
	src/core/timer.cpp
	src/core/timer.hpp
	src/core/window.hpp

	src/core/events/applicationevent.hpp
	src/core/events/event.cpp
	src/core/events/event.hpp
	src/core/events/inputevent.hpp
	
	src/platform/opengl/buffer_opengl.cpp
	src/platform/opengl/buffer_opengl.hpp
	src/platform/opengl/graphicscontext_opengl.cpp
	src/platform/opengl/graphicscontext_opengl.hpp
	src/platform/opengl/program_opengl.cpp
	src/platform/opengl/program_opengl.hpp
	src/platform/opengl/renderer_opengl.cpp
	src/platform/opengl/renderer_opengl.hpp
	src/platform/opengl/vertexarray_opengl.cpp
	src/platform/opengl/vertexarray_opengl.hpp

	src/platform/windows/window_windows.cpp
	src/platform/windows/window_windows.hpp

	src/renderer/buffer.cpp
	src/renderer/buffer.hpp
	src/renderer/framebuffer.hpp
	src/renderer/graphicscontext.cpp
	src/renderer/graphicscontext.hpp
	src/renderer/renderer.cpp
	src/renderer/renderer.hpp
	src/renderer/rendering.hpp
	src/renderer/program.hpp
	src/renderer/program.cpp
	src/renderer/vertexarray.cpp	
	src/renderer/vertexarray.hpp	
	
	src/vin.hpp
	src/vinpch.hpp
)

target_link_libraries(
	vin_engine 
	glad_library 
	glfw 
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