#!/bin/bash
# build.sh - Automates CMake build process (Release mode)

# Configure with CMake (Release mode)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build project
cmake --build build --config Release

echo ""
echo "Build complete. Executable: build/raytracer"
