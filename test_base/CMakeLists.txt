cmake_minimum_required(VERSION 3.10)

project(test_base)

add_library(${PROJECT_NAME} STATIC test_base.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Catch2)
