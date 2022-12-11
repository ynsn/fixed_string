cmake_minimum_required(VERSION 3.23)
project(fixed_string)

set(CMAKE_CXX_STANDARD 20)

add_executable(example examples/example.cpp)
target_include_directories(example PRIVATE include)
