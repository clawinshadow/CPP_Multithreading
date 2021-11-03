cmake_minimum_required(VERSION 3.1)

project(cpp_multithreading)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${PROJECT_NAME}
        main.cpp
        chapter_01/mt_example.cpp)

target_link_libraries(${PROJECT_NAME} pthread)