cmake_minimum_required(VERSION 3.30)

# Check if external tools are installed
include(cmake/check_dependencies.cmake)

project(raspi-control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List of headers
set(HEADERS
	app/App.h
	logger/Logger.h
	logger/LoggerMsg.h
	network_handler/NetworkHandler.h)

# List of sources
set(SOURCES
	app/App.cpp
	logger/Logger.cpp
	network_handler/NetworkHandler.cpp
	src_main/main.cpp)

add_executable(raspi-control ${HEADERS} ${SOURCES})

# Include directories
target_include_directories(raspi-control PRIVATE app logger network_handler
							src_main)
