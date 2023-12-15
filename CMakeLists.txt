cmake_minimum_required(VERSION 3.26)
project(vin_engine)

set(CMAKE_CXX_STANDARD 17)

option(VIN_BUILD_WITH_WAYLAND "Build with Wayland support instead of X11" ON)
option(VIN_BUILD_NDEBUG "Disable debugging information" OFF)
option(VIN_BUILD_TEST "Build vin test app" ON)
option(VIN_BUILD_BENCH "Build vin benchmark" ON)
option(VIN_BUILD_EDITOR "Build vin editor" ON)
option(VIN_BUILD_TOOLS "Build vin tools" ON)

if (WIN32)
    add_compile_definitions(VIN_WIN32)
elseif (APPLE)
    add_compile_definitions(VIN_APPLE)
elseif (UNIX)
    add_compile_definitions(VIN_LINUX)
endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/library)

if(VIN_BUILD_TEST)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif()

if(VIN_BUILD_BENCH)
    #add_subdirectory(${PROJECT_SOURCE_DIR}/bench)
endif()

if(VIN_BUILD_EDITOR)
    add_subdirectory(${PROJECT_SOURCE_DIR}/editor)
endif()

if(VIN_BUILD_TOOLS)
    add_subdirectory(${PROJECT_SOURCE_DIR}/tools)
endif()