cmake_minimum_required(VERSION 3.2)
project(vin-engine)

set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
set(GLAD_SOURCES_DIR ${EXTERNAL_PATH}/glad)

add_subdirectory(${EXTERNAL_PATH}/glad/cmake)
add_subdirectory(${EXTERNAL_PATH}/glfw)

file(GLOB VINLIBRARY_HPP CONFIGURE_DEPENDS "include/*.hpp")
file(GLOB VINLIBRARY_CPP CONFIGURE_DEPENDS "src/*.cpp")

glad_add_library(glad_gl_core_33 SHARED LANGUAGE cpp API gl:core=3.3)

add_library(vin_library ${VINLIBRARY_HPP} ${VINLIBRARY_CPP})

target_link_libraries(vin_library glad_gl_core_33)
target_link_libraries(vin_library glfw)