cmake_minimum_required(VERSION 3.16)
project(benchmarktools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_PARALLEL_LEVEL 12)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Extra warnings
add_compile_options(-Wall -Wextra -pedantic)

message("Compiler options setup...")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h")

message("GLOB done.")

add_executable(benchmarktools main.cpp ${SRC_FILES})

target_include_directories(benchmarktools PRIVATE "${CMAKE_SOURCE_DIR}/headers")

target_link_directories(benchmarktools PRIVATE)
target_link_libraries(benchmarktools PRIVATE)

