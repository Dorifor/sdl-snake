cmake_minimum_required(VERSION 3.22)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
project(snake)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
add_executable(snake src/main.cpp)
target_link_libraries(snake PRIVATE SDL3::SDL3)
