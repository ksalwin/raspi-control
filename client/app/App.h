#pragma once

#include <iostream>
#include <string>
#include "ILogger.h"
#include "Logger.h"
#include "ServerDiscovery.h"

class App {
	private:
		static constexpr const char *class_name {"App"};

		bool is_running {true};
		ILogger *const logger = nullptr;
		ServerDiscovery server_discovery;

		inline void get_user_input(std::string & input);
		inline void log_if_possible(const std::string & msg);
		inline void log_if_possible(const std::string & tag, const std::string & msg);

		void 		handle_user_input(const std::string & input);
		void		show_menu();
	public:
		App();
		App(ILogger *const _logger) : logger(_logger) {}
		void start();
};

inline void App::get_user_input(std::string & input) {
	std::cin >> input;
}

inline void App::log_if_possible(const std::string & msg) {
	if(logger) logger->write(msg);
}

inline void App::log_if_possible(const std::string & tag, const std::string & msg) {
	if(logger) logger->write(tag, msg);
}
