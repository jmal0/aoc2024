cmake_minimum_required(VERSION 3.21)
project(aoc2024)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -pedantic -Wshadow -Wconversion)

add_library(aoc2024 src/read_txt.cpp)

add_executable(day1 src/day1.cpp)
target_link_libraries(day1 aoc2024)

add_executable(day2 src/day2.cpp)
target_link_libraries(day2 aoc2024)

add_executable(day3 src/day3.cpp)
target_link_libraries(day3 aoc2024)
