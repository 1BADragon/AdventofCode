cmake_minimum_required(VERSION 3.20)
project(Advent2015 C CXX)

add_day(1 day1.cpp)
add_day(2 day2.cpp)
add_day(3 day3.cpp)
add_day(4 day4.cpp)
target_link_libraries(Advent2015-day4 md)
add_day(5 day5.cpp)
