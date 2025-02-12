#include "App.h"

void App::handle_input(const std::string & input) {
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
		get_input(input);
		handle_input(input);
	} while(is_running == true);

	logger.write(LoggerMsg::APP_TERMINATE);
}
