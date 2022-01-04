cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)

add_library(traits INTERFACE)
target_include_directories(traits INTERFACE ${CMAKE_CURRENT_LIST_DIR})
