cmake_minimum_required(VERSION 3.26)
project(vin_engine)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(3rdparty/LLGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(vin_engine src/vin.cpp
        include/vin/vin.h
        include/vin/error/error.h
        include/vin/framework/framework.h
        include/vin/framework/frameworkmodule.h
        include/vin/allocator/linearallocator.h
        include/vin/allocator/stdallocator.h
        include/vin/framework/frameworkallocator.h
        include/vin/allocator/generalallocator.h
)
target_include_directories(vin_engine PUBLIC include/)
target_link_libraries(vin_engine LLGL)