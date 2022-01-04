cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

add_library(traits INTERFACE
        impl/on_part_added.hpp
        impl/visitor_trait.hpp
        trait.hpp
        visitor.hpp
        trait_compose.hpp

        # Tree implementation
        tree.hpp
        tree_composite.hpp
        )
set_target_properties(traits PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(traits INTERFACE ${CMAKE_CURRENT_LIST_DIR})
