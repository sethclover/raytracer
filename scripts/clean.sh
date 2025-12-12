#!/bin/bash
# clean.sh - Remove the entire build directory

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

echo "[clean.sh] Cleaning complete."
