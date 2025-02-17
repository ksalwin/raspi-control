enable_testing()

# Add GoogleTest directory
set(GTEST_DIR ../dependencies/third_party/googletest)
add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/googletest)

set(TEST_SOURCES
	test/main.cpp)

add_executable(test-${PROJECT_NAME} ${TEST_SOURCES})

target_link_libraries(test-${PROJECT_NAME} PRIVATE gtest gtest_main)

add_test(NAME test-${PROJECT_NAME} COMMAND test-${PROJECT_NAME})
