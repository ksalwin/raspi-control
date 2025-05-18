# Library
add_library(logger STATIC Logger.cpp)
target_include_directories(logger PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the library to the project executable
target_link_libraries(${PROJECT_NAME} PRIVATE logger)

# Tests
if(TARGET test-${PROJECT_NAME})
	target_sources(test-${PROJECT_NAME} PRIVATE test-Logger.cpp)
	# Link library to project test executable
	target_link_libraries(test-${PROJECT_NAME} PRIVATE logger)
endif()
