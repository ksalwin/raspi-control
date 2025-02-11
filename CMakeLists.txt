cmake_minimum_required(VERSION 3.30)

# Check if external tools are installed
include(cmake/check_dependencies.cmake)

project(raspi-control)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List of headers
set(HEADERS
	logger/logger.h)

# List of sources
set(SOURCES
	logger/logger.cpp
	src_main/main.cpp)

add_executable(raspi-control ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(raspi-control PRIVATE src_main logger)
