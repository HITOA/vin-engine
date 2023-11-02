cmake_minimum_required(VERSION 3.26)
project(vin_engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    set(CMAKE_CXX_FLAGS "/W4")
else()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if (VIN_BUILD_WITH_WAYLAND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lwayland-client -lwayland-egl")
endif()

add_executable(vin_test main.cpp)
target_link_libraries(vin_test vin_engine)