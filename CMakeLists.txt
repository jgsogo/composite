cmake_minimum_required(VERSION 3.20)
project(traits C CXX)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

add_executable(executable
        main.cpp
        depth.cpp
        depth.hpp
        autonum.hpp
        autonum.cpp
        )
target_link_libraries(executable PUBLIC traits)

add_subdirectory(tests)
