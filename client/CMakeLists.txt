cmake_minimum_required(VERSION 3.30)

# Check if external tools are installed
include(cmake/check_dependencies.cmake)

project(raspi-control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build config
include(cmake/build_config.cmake)
