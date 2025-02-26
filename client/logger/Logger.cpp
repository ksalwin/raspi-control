#include "Logger.h"
#include <ctime>
#include <iomanip>	// Needed for put_time
#include <iostream>

#include "LoggerMsg.h"

Logger::Logger() {	
	logfile.open("log", std::fstream::out);
}

Logger::~Logger() {
	if(logfile.is_open() == true)
		logfile.close();
}

std::string Logger::get_timestamp() {
	// Get current time as number of seconds since 1 Jan 1970
	time_t raw_time = std::time(nullptr);

	// Convert raw time to time structure
	// Structure defined in library, here received the pointer
	std::tm *time_struct = std::localtime(&raw_time);

	// Extract time_struct values and convert to string
	std::ostringstream time_stream;
	time_stream << "[" << std::put_time(time_struct, "%Y-%m-%d %H:%M:%S") << "]";

	return time_stream.str();
}

bool Logger::is_logfile_open()
{
	return logfile.is_open();
}

void Logger::write(const std::string & msg)
{
	std::string log_msg { get_timestamp() + ' ' + msg + '\n' };

	std::cout	<< log_msg;
	logfile		<< log_msg;
}

void Logger::write(const std::string & tag, const std::string & content)
{
	std::string log_msg { get_timestamp() + ' ' + tag + ": " + content + '\n' };

	std::cout	<< log_msg;
	logfile		<< log_msg;
}
