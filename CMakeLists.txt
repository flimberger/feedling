cmake_minimum_required(VERSION 3.0)

project(feedling)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(feedling_VERSION_MAYOR  "0")
set(feedling_VERSION_MINOR  "1")
set(feedling_VERSION_PATCH  "0-prealpha")

find_package(Qt5 REQUIRED COMPONENTS
    Network
    Widgets)

if (("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    OR
    ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
    add_definitions(
        -fno-rtti
        -fno-exceptions
    )
    if ("${CMAKE_CXX_COMPILER}" STREQUAL "Clang")
        add_definitions(
            -fcolor-diagnostics
            -Weverything
            -Wno-c++98-compat
            -Wno-padded
            -Wno-unknown-pragmas # Thanks, Qt
        )
    else()
        add_definitions(
            -Wall
            -Wpedantic
        )
    endif()
endif()

add_subdirectory(src)
