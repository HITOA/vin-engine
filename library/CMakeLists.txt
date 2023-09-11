cmake_minimum_required(VERSION 3.26)
project(vin_engine)

set(CMAKE_CXX_STANDARD 17)

add_library(vin_engine src/vin.cpp
        include/vin/vin.h
        include/vin/allocator/allocator.h
        include/vin/allocator/tempalloc.h
        src/allocator/tempalloc.cpp
        include/vin/allocator/persistentalloc.h
        src/allocator/persistentalloc.cpp
        include/vin/allocator/stdallocator.h
)
target_include_directories(vin_engine PUBLIC include/)