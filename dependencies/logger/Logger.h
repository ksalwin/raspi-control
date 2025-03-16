#pragma once

#include <ctime>
#include <fstream>
#include <string>

#include "ILogger.h"
#include "LoggerMsg.h"

class Logger : public ILogger {
	private:
		std::ofstream logfile;
		std::string	get_timestamp() const;

	public:
		Logger();
		~Logger();

		bool is_logfile_open() const;

		void write(const std::string & msg) override;
		void write(const std::string & tag, const std::string & msg) override;
};
