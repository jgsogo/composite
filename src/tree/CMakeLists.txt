add_library(tree INTERFACE)
set_target_properties(tree PROPERTIES
        PUBLIC_HEADER "visitor.hpp;visitor_tree.hpp;visitor_wrapper_cast.hpp;tree.hpp;tree_composite.hpp"
        )
target_link_libraries(tree
        INTERFACE
        impl
        )
install(TARGETS tree
        PUBLIC_HEADER DESTINATION include/composite/tree
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
