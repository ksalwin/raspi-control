#pragma once

#include <string>
#include "logger.h"

class App {
	private:
		bool is_running = true;
		Logger logger;

		void handle_input(const std::string & input);
		void show_menu();
	public:
		void start();
};
