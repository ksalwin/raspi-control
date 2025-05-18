cmake_minimum_required(VERSION 3.25)
project(raspi-control-server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executables
add_executable(${PROJECT_NAME})

# Subdirectories
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(network)

# Dependencies
add_subdirectory(../shared/logger ${CMAKE_BINARY_DIR}/shared/logger)
