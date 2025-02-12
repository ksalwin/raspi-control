#pragma once

#include <ctime>
#include <fstream>
#include <string>

#include "LoggerMsg.h"

class Logger
{
	private:
		std::ofstream logfile;

		void print_to_console	(const char *time, const char *msg);
		void print_to_file		(const char *time, const char *msg);
		
	public:
		Logger();
		~Logger();

		void write(const std::string & msg);
		void write(const std::string & msg, const std::string & value);
};
