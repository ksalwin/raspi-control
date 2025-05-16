#pragma once

class ILogger {
	public:
		virtual void info(const std::string& ms) = 0;
		virtual void write(const std::string & msg) = 0;
		virtual void write(const std::string & tag, const std::string & content) = 0;
		virtual ~ILogger() = default;
};
