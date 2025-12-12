# CMake Parent File Requirements
cmake_minimum_required(VERSION 3.31...4.2)

set(CMAKE_CXX_STANDARD 20)

# Project Definition
project(
        SimpleSTLTest
)

add_executable(SimpleSTL-Test
        Test.cpp
)

target_include_directories(SimpleSTL-Test PUBLIC ../include)

target_link_libraries(SimpleSTL-Test SimpleSTL)