cmake_minimum_required(VERSION 3.15)
project(composite
        LANGUAGES C CXX
        VERSION 0.2.1
        DESCRIPTION "Data structures with composite nodes")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

option(BUILD_TESTS "Also build the tests" ON)
if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
