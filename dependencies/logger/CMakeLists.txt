# Create a static library
add_library(logger STATIC Logger.cpp)

# Set the correct include directory for both the library and any target that links to it
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the library to the project executable
target_link_libraries(${PROJECT_NAME} PRIVATE logger)

# Tests
if(TARGET test-${PROJECT_NAME})
	target_sources(test-${PROJECT_NAME} PRIVATE Logger.cpp test-Logger.cpp)
	# Link library to project test executable
	target_link_libraries(test-${PROJECT_NAME} PRIVATE logger)
endif()
