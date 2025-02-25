#pragma once

#include <ctime>
#include <fstream>
#include <string>

#include "ILogger.h"
#include "LoggerMsg.h"

class Logger : public ILogger {
	private:
		std::ofstream logfile;

		std::string	get_timestamp();
		void print_to_console	(const char *time, const char *msg);
		void print_to_file		(const char *time, const char *msg);

	public:
		Logger();
		~Logger();

		bool is_logfile_open();

		void write(const std::string & msg) override;
		void write(const std::string & tag, const std::string & content) override;
};
