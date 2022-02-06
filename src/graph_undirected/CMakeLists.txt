
add_library(graph_undirected INTERFACE)
set_target_properties(graph_undirected PROPERTIES
        PUBLIC_HEADER "visitor.hpp;visitor_graph.hpp;visitor_wrapper_cast.hpp;graph_undirected.hpp;graph_undirected_composite.hpp"
        )
target_include_directories(graph_undirected INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(graph_undirected
        INTERFACE
        impl
        )
install(TARGETS graph_undirected
        PUBLIC_HEADER DESTINATION include/composite/graph_undirected
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
