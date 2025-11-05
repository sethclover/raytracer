#!/bin/bash
# build.sh - Automates CMake build process

# Create build directory if not exists
mkdir -p build

# Enter build directory
cd build

# Run CMake configuration step
cmake ..

# Build project
cmake --build .

# Return to project root
cd ..
