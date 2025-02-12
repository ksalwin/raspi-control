#include "App.h"

void App::cmd_discover_rpi() {
	// todo: Start rpi listener in backgrond
	// ...

	// Send broadcast UDP packet to detect rpi
	net_handler.send_broadcast_udp_packet();
}

void App::handle_user_input(const std::string & input) {
	if		(input == "d" || input == "detect-rpi")
	{
		cmd_discover_rpi();
	}
	else if	(input == "q" || input == "quit")
	{
		std:: cout << "Terminating application\n";
		is_running = false;
	}
	else { /* Undefined cmd */ }
}

void App::show_menu() {
	std::cout	<< "=== Main Menu ===\n"
				<< "'d' or 'detect-rpi' to detect Raspberr Pi on local network.\n"
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
