#include <gtest/gtest.h>

#include <fstream>		// is_open()
#include <regex>		// reg exp
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

TEST(TestLogger, Write_to_logfile)
{
	std::string test_txt {"Some text 123"};
	std::string readback {};
	std::regex pattern (R"(\[\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\] Some text 123)");

	// Write
	{
		Logger logger;
		logger.write(test_txt);
	}

	// Read back
	std::ifstream logfile(file_n);
	ASSERT_EQ(logfile.is_open(), true) << "[ERROR] File not opened";

	getline(logfile, readback);
	logfile.close();

	// Check if readback matches pattern
	ASSERT_TRUE(std::regex_match(readback, pattern)) << "[ERROR] write != readback";
}
