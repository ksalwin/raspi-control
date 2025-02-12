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

void Logger::write(const std::string & msg) {
	// Get current time as number of seconds since 1 Jan 1970
	time_t raw_time = std::time(nullptr);

	// Convert raw time to time structure
	// Structure defined in library, here received the pointer
	std::tm *time_struct = std::localtime(&raw_time);

	// Print log message
	std::cout	<< '['
				<< std::put_time(time_struct, "%Y-%m-%d %H:%M:%S")
				<< "] "
				<< msg
				<< '\n';

	this->logfile	<< '['
				<< std::put_time(time_struct, "%Y-%m-%d %H:%M:%S")
				<< "] "
				<< msg
				<< '\n';
}
