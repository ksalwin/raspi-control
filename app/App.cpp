#include "App.h"

void App::cmd_discover_rpi() {
	//broadcast UDP packet to local network
}

void App::handle_user_input(const std::string & input) {
	if(input == "q" || input == "quit" || input == "exit") {
		std:: cout << "Terminating application\n";
		is_running = false;
	}
}

void App::show_menu() {
	std::cout	<< "=== Main Menu ===\n"
				<< "'q' or 'quit' or 'exit' to quit.\n";
}

void App::start() {
	std::string input;

	logger.write(LoggerMsg::APP_START);

	do
	{
		show_menu();
		get_user_input(input);
		log_user_input(input);
		handle_user_input(input);
	} while(is_running == true);

	logger.write(LoggerMsg::APP_TERMINATE);
}
