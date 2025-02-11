cmake_minimum_required(VERSION 3.30)

# Check if external tools are installed
include(cmake/check_dependencies.cmake)

project(raspi-control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List of headers
set(HEADERS
	app/App.h
	logger/logger.h)

# List of sources
set(SOURCES
	app/App.cpp
	logger/logger.cpp
	src_main/main.cpp)

add_executable(raspi-control ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(raspi-control PRIVATE app logger src_main)
