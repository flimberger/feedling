cmake_minimum_required(VERSION 3.9)

project(Catch2 CXX)

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
