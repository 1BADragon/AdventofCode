cmake_minimum_required(VERSION 3.20)
project(Advent C CXX)

include(advent.cmake)

set(CMAKE_CXX_STANDARD 20)

option(BUILD_2015 "Build year 2015 solutions" on)
option(BUILD_2021 "Build year 2015 solutions" on)

add_executable(advent runner/advent.cpp)
target_link_libraries(advent dl)
set_target_properties(advent
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(2015)
add_subdirectory(2021)
