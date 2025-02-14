#!/bin/bash

set -e # Exit on error

# Go to client app dir
cd client/

BUILD_DIR="build"

# Create build directory if it doesn't exist
[ -d "$BUILD_DIR" ] || mkdir "$BUILD_DIR"

# Run CMake configuration
cmake -B "$BUILD_DIR" -D ENABLE_TESTS=ON

# Run CMake build
cmake --build "$BUILD_DIR"

# Run tests
cd "$BUILD_DIR"
ctest
