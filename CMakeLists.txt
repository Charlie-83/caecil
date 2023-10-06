cmake_minimum_required(VERSION 3.1.0)
project(Engine VERSION 0.1.0)
set (CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -DDEBUG")

find_package(spdlog REQUIRED)

include_directories (
    source
)

set (SOURCES
    source/lex.cpp
)
add_library(caecil ${SOURCES})
add_executable(main source/main.cpp)
target_link_libraries(main caecil spdlog::spdlog)
