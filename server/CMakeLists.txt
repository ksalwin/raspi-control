cmake_minimum_required(VERSION 3.25)
project(raspi-control-server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executables
add_executable(${PROJECT_NAME})

# Subdirectories
add_subdirectory(main)

# Dependencies
add_subdirectory(../dependencies/logger ${CMAKE_BINARY_DIR}/dependencies/logger)
