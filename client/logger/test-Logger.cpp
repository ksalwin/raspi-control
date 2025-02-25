#include <gtest/gtest.h>

#include <fstream>		// is_open()
#include <unistd.h>		// stat()

#include "Logger.h"


constexpr auto file_n = "log";

//---TESTS--------------------------------------------------------------------------------

TEST(TestLogger, Logfile_open_and_close)
{
	// stat is a struct defined in <sys/stat.h>.
	// It holds metadata about a file (size, type, mod date, permissions).
	// Updated b calling stat() function.
	struct stat file_info;

	// Constructor creates log file
	Logger logger;

	// Test: Check if file was created 
	auto file_exists = stat(file_n, &file_info) == 0 ? true : false;
	EXPECT_EQ(file_exists, true) << "[ERROR] Logfile not created";

	// Test: Check if file opened
	ASSERT_EQ(logger.is_logfile_open(), true) << "[ERROR] Logfile not opened";

	// Test: Check if file opened for write
	auto file_opened_w = file_info.st_mode & S_IWUSR ? true : false;
	EXPECT_EQ(file_opened_w, true) << "[ERROR] Logfile not opened for write";
}
