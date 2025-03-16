#include "App.h"

void App::handle_user_input(const std::string & input) {
	if		(input == "d" || input == "detect-rpi")
	{
		server_discovery.start();
	}
	else if	(input == "q" || input == "quit")
	{
		server_discovery.stop();
		is_running = false;
	}
	else { /* Undefined cmd */ }
}

void App::show_menu() {
	std::cout	<< "=== Main Menu ===\n"
				<< "'d' or 'detect-rpi' to detect Raspberry Pi on local network.\n"
				<< "'q' or 'quit' or 'exit' to quit.\n";
}

void App::start() {
	std::string input;

	log_if_possible(LoggerMsg::APP_START);

	do
	{
		show_menu();
		get_user_input(input);
		log_if_possible(LoggerMsg::USER_INPUT, input);
		handle_user_input(input);
	} while(is_running == true);

	log_if_possible(LoggerMsg::APP_TERMINATE);
}
