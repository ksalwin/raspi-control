#include <gtest/gtest.h>

#include <fstream>		// is_open()
#include <regex>		// reg exp
#include <unistd.h>		// stat()

#include "Logger.h"


constexpr auto file_n = "log";

//---TESTS--------------------------------------------------------------------------------

TEST(TestLogger, Open_logfile)
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

TEST(TestLogger, Close_logfile)
{
	// 1: Try to open the file - check if it was closed properly in prev test
	// 2: Open and close the file using Logger
	// 3: Try to open the file again - check if it was closed properly by Logger

	// 1
	// If the destructor didn’t close it, you might see a failure.
	// std::ios::app puts position at the end of a fille.
    std::ofstream logfile1(file_n, std::ios::app);
    EXPECT_TRUE(logfile1.is_open()) << "[ERROR] File not closed properly in prev test";
    logfile1.close();

	// 2
	{
		Logger logger;
	}

	// 3
    std::ofstream logfile(file_n, std::ios::app);
    EXPECT_TRUE(logfile.is_open()) << "[ERROR] File not closed properly by Logger";
    logfile.close();
}

TEST(TestLogger, Write_to_logfile)
{
	const std::string test_txt {"Some text 123"};
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
