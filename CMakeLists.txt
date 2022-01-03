cmake_minimum_required(VERSION 3.20)
project(traits C CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(traits INTERFACE
        src/impl/on_part_added.hpp
        src/impl/visitor_trait.hpp
        src/trait.hpp
        src/visitor.hpp
        src/trait_compose.hpp
        )
set_target_properties(traits PROPERTIES LINKER_LANGUAGE CXX)

add_executable(executable
        main.cpp
        depth.cpp
        depth.hpp
        autonum.hpp
        autonum.cpp
        )
target_link_libraries(executable PUBLIC traits)
