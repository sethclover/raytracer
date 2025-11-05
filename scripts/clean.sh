#!/bin/bash
# clean.sh - Remove the entire build directory and .ppm files

set -e

# Define build directory relative to project root
BUILD_DIR="build"

# Check if build directory exists and remove it
if [ -d "$BUILD_DIR" ]; then
  echo "[clean.sh] Removing $BUILD_DIR..."
  rm -rf "$BUILD_DIR"
  echo "[clean.sh] $BUILD_DIR removed."
else
  echo "[clean.sh] Nothing to clean. ($BUILD_DIR does not exist)"
fi

# Remove all .ppm files in the project directory and subdirectories
echo "[clean.sh] Removing .ppm files..."
find . -type f -name "*.ppm" -exec rm -f {} +

echo "[clean.sh] Cleaning complete."
