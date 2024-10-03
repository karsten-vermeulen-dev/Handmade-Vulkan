cmake_minimum_required(VERSION 3.10)

project(Handmade)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_FILES 
${CMAKE_SOURCE_DIR}/src/Input.h
${CMAKE_SOURCE_DIR}/src/Screen.h)

set(SOURCE_FILES 
${CMAKE_SOURCE_DIR}/src/Input.cpp
${CMAKE_SOURCE_DIR}/src/Main.cpp
${CMAKE_SOURCE_DIR}/src/Screen.cpp)

add_executable(Handmade ${INCLUDE_FILES} ${SOURCE_FILES})

target_include_directories(Handmade PRIVATE ${CMAKE_SOURCE_DIR}/src/GLFW)
target_include_directories(Handmade PRIVATE ${CMAKE_SOURCE_DIR}/src/Vulkan)

target_link_libraries(Handmade 
${CMAKE_SOURCE_DIR}/lib/glfw3.lib)