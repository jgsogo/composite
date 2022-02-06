cmake_minimum_required(VERSION 3.15)

# Some common utilities
add_library(impl INTERFACE)
set_target_properties(impl PROPERTIES
        PUBLIC_HEADER "impl/is_add_function.hpp;impl/refl.hpp"
        )
target_include_directories(impl INTERFACE ${CMAKE_CURRENT_LIST_DIR})
install(TARGETS impl
        PUBLIC_HEADER DESTINATION include/composite
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Tree
add_library(tree INTERFACE)
set_target_properties(tree PROPERTIES
        PUBLIC_HEADER "tree/visitor.hpp;tree/visitor_tree.hpp;tree/visitor_wrapper_cast.hpp;tree.hpp;tree_composite.hpp"
        )
target_link_libraries(tree
        INTERFACE
        impl
        )
install(TARGETS tree
        PUBLIC_HEADER DESTINATION include/composite
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Graog
add_library(graph INTERFACE)
set_target_properties(graph PROPERTIES
        PUBLIC_HEADER "graph_undirected/visitor.hpp;graph_undirected/visitor_graph.hpp;graph_undirected/visitor_wrapper_cast.hpp;graph_undirected.hpp;graph_undirected_composite.hpp"
        )
target_link_libraries(graph
        INTERFACE
        impl
        )
install(TARGETS graph
        PUBLIC_HEADER DESTINATION include/composite
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

# Target to gather them all
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME}
        INTERFACE
        tree
        graph
        )
