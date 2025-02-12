#pragma once

#include <iostream>
#include <string>
#include "logger.h"

class App {
	private:
		bool is_running = true;
		Logger logger;

		inline void	get_input(std::string & input)	{ std::cin >> input; }
		void 		handle_input(const std::string & input);
		void		show_menu();
	public:
		void start();
};
