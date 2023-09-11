cmake_minimum_required(VERSION 3.26)
project(vin_engine)

set(CMAKE_CXX_STANDARD 17)

find_library(VIN_ENGINE vin_engine)

add_executable(vin_test main.cpp)
target_link_libraries(vin_test vin_engine)