cmake_minimum_required(VERSION 3.2)
project(vin)

set(CMAKE_CXX_STANDARD 17)

set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/3rdparty)
set(GLAD_SOURCES_DIR ${EXTERNAL_PATH}/glad)

add_subdirectory(${EXTERNAL_PATH}/glad/cmake)
add_subdirectory(${EXTERNAL_PATH}/glfw)

file(GLOB VINENGINE_HPP CONFIGURE_DEPENDS "include/*.hpp")
file(GLOB VINENGINE_CPP CONFIGURE_DEPENDS "src/*.cpp")

glad_add_library(glad_library STATIC API gl:core=4.6)

add_executable(vin_engine ${VINENGINE_HPP} ${VINENGINE_CPP})

target_include_directories(vin_engine PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(vin_engine glad_library glfw)