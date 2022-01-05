cmake_minimum_required(VERSION 3.15)


find_package(Catch2 REQUIRED)

include(CTest)
enable_testing()

add_executable(unit_tests
        main.cpp

        # Trees
        test_tree/test_simple/test_node_simple.cpp
        test_tree/test_simple/test_visitor.cpp
        test_tree/test_simple/test_add_function.cpp
        test_tree/test_composite/test_composite.cpp
        test_tree/test_composite/test_composite3.cpp

        # Graph undirected
        test_graph_undirected/test_simple/test_node_create.cpp
        test_graph_undirected/test_simple/test_visitor.cpp
        test_graph_undirected/test_simple/test_add_function.cpp
        test_graph_undirected/test_composite/test_composite.cpp
        test_graph_undirected/test_composite/test_composite3.cpp
        )
target_link_libraries(unit_tests
        Catch2::Catch2
        traits
        )

# Load and use the .cmake file provided by Catch so all the test cases
# are made available to CTest.
include(Catch)
catch_discover_tests(unit_tests)

install(
        TARGETS unit_tests
        RUNTIME DESTINATION bin
)
