cmake_minimum_required(VERSION 3.10)

project(Handmade)

set(CMAKE_CXX_STANDARD 17)

add_executable(Handmade src/Main.cpp)

target_include_directories(Handmade PRIVATE ${CMAKE_SOURCE_DIR}/src/Vulkan)

