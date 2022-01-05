cmake_minimum_required(VERSION 3.15)
project(composite
        LANGUAGES C CXX
        VERSION 0.1
        DESCRIPTION "Data structures with composite nodes")

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests)
