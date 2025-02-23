#include "Logger.h"
#include <ctime>
#include <iomanip>	// Needed for put_time
#include <iostream>

#include "LoggerMsg.h"

Logger::Logger() {	
	std::cout << "Logger constructor\n";
	this->logfile.open("log", std::fstream::out);
}

Logger::~Logger() {
	std::cout << "Logger destructor\n";
	this->logfile.close();
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

void Logger::write(const std::string & msg)
{
	std::string timestamp { get_timestamp() };

	// Print log message
	std::cout	<< timestamp << ' '
				<< msg
				<< '\n';

	logfile		<< timestamp << ' '
				<< msg
				<< '\n';
}

void Logger::write(const std::string & tag, const std::string & content)
{
	std::string timestamp { get_timestamp() };

	// Print log message
	std::cout	<< timestamp << ' '
				<< tag << ": " << content << '\n';

	logfile		<< timestamp << ' '
				<< tag << ": " << content << '\n';
}
