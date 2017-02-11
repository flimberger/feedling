cmake_minimum_required(VERSION 3.0)

project(feedling)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(feedling_VERSION_MAJOR  "0")
set(feedling_VERSION_MINOR  "1")
set(feedling_VERSION_PATCH  "0-prealpha")

find_package(Qt5 REQUIRED COMPONENTS
    Network
    Widgets)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    OR
    ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
    add_compile_options(
        -fno-rtti
        -fno-exceptions
    )
    add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050500 -DQT_DEPRECATED_WARNINGS)
    if ("${CMAKE_CXX_COMPILER}" STREQUAL "Clang")
        add_compile_options(
            -fcolor-diagnostics
            -Weverything
            -Wno-c++98-compat
            -Wno-padded
            -Wno-unknown-pragmas # Thanks, Qt
        )
    else()
        add_compile_options(
            -Wall
            -Wpedantic
        )
    endif()
endif()

add_subdirectory(src)
add_subdirectory(test)
