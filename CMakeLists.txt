cmake_minimum_required(VERSION 3.15)
project(raytracer LANGUAGES CXX)

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Collect all source files from src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Define the executable target
add_executable(raytracer ${SOURCES})

# Specify public include directories (for #include<raytracer/vec3.hpp>, etc.)
target_include_directories(raytracer
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src           # for local includes if needed
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include       # for all headers in include/raytracer
        ${CMAKE_SOURCE_DIR}/include/raytracer
)

# (Optional but recommended) Set compiler warnings per target
if(MSVC)
    target_compile_options(raytracer PRIVATE /W4 /WX)
else()
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# (Optional) Export compile_commands.json for tools/IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
