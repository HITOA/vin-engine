cmake_minimum_required(VERSION 3.26)
project(vin_engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if (VIN_BUILD_WITH_WAYLAND)
    set(GLFW_BUILD_WAYLAND ON)
    set(GLFW_BUILD_X11 OFF)
    set(BGFX_BUILD_WITH_WAYLAND ON)
else()
    set(GLFW_BUILD_WAYLAND OFF)
    set(GLFW_BUILD_X11 ON)
    set(BGFX_BUILD_WITH_WAYLAND OFF)
endif()

add_subdirectory(3rdparty/bgfx.cmake)
add_subdirectory(3rdparty/glfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(vin_engine
        3rdparty/tlsf/tlsf.h
        3rdparty/tlsf/tlsf.c
        include/vin/vin.h
        include/vin/core/thread/threadpool.h
        src/threadpool.cpp
        include/vin/core/error/error.h
        include/vin/core/memory/memutils.h
        include/vin/core/memory/stackallocator.h
        include/vin/core/memory/allocator.h
        include/vin/core/memory/mallocator.h
        include/vin/core/memory/doublebufferallocator.h
        include/vin/core/memory/memorymanager.h
        include/vin/core/memory/segregator.h
        include/vin/core/memory/tlsfallocator.h
        include/vin/core/templates/ref.h
        include/vin/core/templates/stdcontainers.h
        include/vin/core/memory/memoryresource.h
        src/memorymanager.cpp
        include/vin/core/memory/stdallocator.h
        include/vin/application/module.h
        include/vin/application/dependencylist.h
        include/vin/application/application.h
        src/application.cpp
        include/vin/modules/window/windowmodule.h
        src/windowmodule.cpp
        include/vin/core/templates/event.h
        include/vin/modules/rendering/renderingmodule.h
        src/renderingmodule.cpp
        include/vin/core/templates/utils.h
        include/vin/vfs/vfs.h
        include/vin/vfs/filesystem.h
        include/vin/vfs/file.h
        src/vfs.cpp
        include/vin/vfs/native/nativefilesystem.h
        src/nativefilesystem.cpp
        include/vin/vfs/native/nativefile.h
        src/nativefile.cpp
        include/vin/resource/resourcemanager.h
        include/vin/resource/resourcehandle.h
        include/vin/resource/resourceloader.h
        include/vin/core/logger/logger.h
        src/logger.cpp
        include/vin/core/logger/formatter.h
        include/vin/core/templates/hash.h
        src/resourcemanager.cpp
)
target_include_directories(vin_engine PUBLIC include/)
target_include_directories(vin_engine PUBLIC 3rdparty/tlsf)
target_link_libraries(vin_engine bgfx glfw)

if (VIN_BUILD_NDEBUG)
    target_compile_definitions(vin_engine PUBLIC "VIN_BUILD_NDEBUG")
endif()

if (UNIX AND NOT APPLE)
    target_compile_definitions(vin_engine PUBLIC "VIN_LINUX")
    if (VIN_BUILD_WITH_WAYLAND)
        target_compile_definitions(vin_engine PUBLIC "VIN_USE_WAYLAND")
    else ()
        target_compile_definitions(vin_engine PUBLIC "VIN_USE_X11")
    endif()
endif()

if (WIN32)
    target_compile_definitions(vin_engine PUBLIC "VIN_WIN32")
endif()