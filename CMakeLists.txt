cmake_minimum_required(VERSION 3.16)
project(benchmarktools LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_PARALLEL_LEVEL 12)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# gcc, clang
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# MSVC
if (MSVC)
    add_compile_options(/W4 /WX)
endif()

message("Compiler options setup...")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/benchtools/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/benchtools/headers/*.h")

message("GLOB done.")

add_executable(benchmarktools example.cpp ${SRC_FILES})

target_include_directories(benchmarktools PRIVATE "${CMAKE_SOURCE_DIR}/benchtools/headers")

target_link_directories(benchmarktools PRIVATE)
target_link_libraries(benchmarktools PRIVATE)

