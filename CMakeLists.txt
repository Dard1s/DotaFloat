cmake_minimum_required(VERSION 3.20)
project(DotaFloat LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_AVX2 "Enable AVX2 optimizations" ON)
option(ENABLE_LTO "Enable LTO" OFF)

if(MSVC)
    add_compile_options(/W4 /permissive- /MP /EHsc /bigobj)
    if(ENABLE_AVX2)
        add_compile_options(/arch:AVX2)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(cmake/toolchain.cmake)

add_subdirectory(src/loader)
add_subdirectory(src/core)

set_target_properties(dota_core PROPERTIES OUTPUT_NAME "dotafloat.core")
set_target_properties(dotafloat_loader PROPERTIES OUTPUT_NAME "dotafloat_loader")
