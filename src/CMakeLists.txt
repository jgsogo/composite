cmake_minimum_required(VERSION 3.15)

# Some common utilities
add_library(impl INTERFACE)
set_target_properties(impl PROPERTIES
        PUBLIC_HEADER "impl/is_add_function.hpp;impl/refl.hpp"
        )
target_include_directories(impl INTERFACE ${CMAKE_CURRENT_LIST_DIR})
install(TARGETS impl
        PUBLIC_HEADER DESTINATION include/composite/impl
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

add_subdirectory(tree)
add_subdirectory(graph_undirected)

# Target to gather them all
add_library(${PROJECT_NAME} INTERFACE)
target_link_libraries(${PROJECT_NAME}
        INTERFACE
        tree
        graph_undirected
        )
