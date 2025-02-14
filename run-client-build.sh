#!/bin/bash

set -e # Exit on error

# Go to client app dir
cd client/

BUILD_DIR="build"

# Create build directory if it doesn't exist
[ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"

# Run CMake configuration
cmake -B "$BUILD_DIR"

# Run CMake build
cmake --build "$BUILD_DIR"

echo
echo "Build completed. Run executable with: ./client/$BUILD_DIR/raspi-control"
echo
