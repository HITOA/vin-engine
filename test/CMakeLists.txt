cmake_minimum_required(VERSION 3.26)
project(vin_engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -lwayland-client -lwayland-egl")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(vin_test main.cpp)
target_link_libraries(vin_test vin_engine)