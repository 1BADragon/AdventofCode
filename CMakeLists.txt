cmake_minimum_required(VERSION 3.20)
project(Advent C CXX)

include(advent.cmake)

add_executable(advent runner/advent.cpp)
target_link_libraries(advent dl)
set_target_properties(advent
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(2015)
add_subdirectory(2021)
