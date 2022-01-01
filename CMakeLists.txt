cmake_minimum_required(VERSION 3.20)
project(traits)

set(CMAKE_CXX_STANDARD 20)

add_executable(traits
        main.cpp
        trait.hpp
        depth.cpp
        depth.hpp
        autonum.hpp
        autonum.cpp
        )
