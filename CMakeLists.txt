cmake_minimum_required(VERSION 3.26)
project(vin_engine)

set(CMAKE_CXX_STANDARD 17)

set(3RDPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

add_subdirectory(${PROJECT_SOURCE_DIR}/library)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)
add_subdirectory(${PROJECT_SOURCE_DIR}/bench)