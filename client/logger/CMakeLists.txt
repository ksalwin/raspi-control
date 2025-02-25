target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Build
target_sources(${PROJECT_NAME} PRIVATE Logger.cpp)

# Test
if(TARGET test-${PROJECT_NAME})
	target_sources(test-${PROJECT_NAME} PRIVATE Logger.cpp test-Logger.cpp)
endif()
