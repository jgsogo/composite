cmake_minimum_required(VERSION 3.20)


find_package(Catch2 REQUIRED)

include(CTest)
enable_testing()

add_executable(unit_tests
        main.cpp
        test_trait_simple.cpp
        test_visitor.cpp
        test_trait_addpart.cpp
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
