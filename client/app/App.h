#pragma once

#include <iostream>
#include <string>
#include "ILogger.h"
#include "Logger.h"
#include "NetworkHandler.h"

class App {
	private:
		bool is_running {true};
		ILogger & logger;
		NetworkHandler net_handler;

		inline void get_user_input(std::string & input);
		inline void log_user_input(const std::string & input);

		void		cmd_discover_rpi();
		void 		handle_user_input(const std::string & input);
		void		show_menu();
	public:
		App(ILogger & _logger) : logger(_logger) {}
		void start();
};

inline void App::get_user_input(std::string & input) {
	std::cin >> input;
}

inline void App::log_user_input(const std::string & input) {
	logger.write(LoggerMsg::USER_INPUT, input);
}
