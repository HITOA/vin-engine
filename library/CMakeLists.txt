cmake_minimum_required(VERSION 3.26)
project(vin_engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(3rdparty/bgfx.cmake)
add_subdirectory(3rdparty/glfw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(vin_engine src/vin.cpp
        include/vin/vin.h
        include/vin/core/allocator/linearallocator.h
        include/vin/core/allocator/stdallocator.h
        include/vin/core/thread/threadpool.h
        src/threadpool.cpp
        include/vin/core/allocator/buddyallocator.h
        include/vin/core/memory/memutils.h
        include/vin/core/memory/memorymanager.h
        src/memorymanager.cpp
)
target_include_directories(vin_engine PUBLIC include/)
target_link_libraries(vin_engine bgfx glfw)