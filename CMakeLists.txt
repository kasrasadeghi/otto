cmake_minimum_required(VERSION 3.8.2)

project(otto CXX)

find_package(Curses REQUIRED)

add_compile_options(-pedantic -Wall -Weffc++)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

link_libraries(${CURSES_LIBRARIES})

add_executable(otto main.cpp)
