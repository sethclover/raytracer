cmake_minimum_required(VERSION 3.15)
project(raytracer LANGUAGES CXX)

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all source files from src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Define the executable target
add_executable(raytracer ${SOURCES})

# Specify public include directories
target_include_directories(raytracer
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include       # for all headers in include/raytracer
        ${CMAKE_SOURCE_DIR}/include/raytracer
)

# Set compiler warnings per target
if(MSVC)
    target_compile_options(raytracer PRIVATE /W4 /WX)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
