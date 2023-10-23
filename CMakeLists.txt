cmake_minimum_required(VERSION 3.12)
project(pizip)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "-lstdc++fs -std=c++17")
# Add the cxxopts library for parsing command-line arguments
add_subdirectory(external/cxxopts)

add_executable(pizip main.cc)

# Link the cxxopts library
target_link_libraries(pizip PRIVATE cxxopts)

