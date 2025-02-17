cmake_minimum_required(VERSION 3.30)

project(raspi-control)

# Check if external tools are installed
include(cmake/check_dependencies.cmake)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build config
add_executable(${PROJECT_NAME})
add_subdirectory(app)
add_subdirectory(logger)
add_subdirectory(main)
add_subdirectory(network_handler)

# Tests
if (ENABLE_TESTS)
	include(cmake/test.cmake)
endif()
