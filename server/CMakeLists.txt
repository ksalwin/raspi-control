cmake_minimum_required(VERSION 3.30)

project(raspi-control)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executables
add_executable(${PROJECT_NAME})

if (BUILD_TESTS)
	add_executable(test-${PROJECT_NAME})
	include(cmake/test.cmake)
endif()

# Subdirectories
add_subdirectory(main)

# Dependencies
add_subdirectory(../dependencies/logger ${CMAKE_BINARY_DIR}/dependencies/logger)
