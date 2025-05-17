// app/App.cpp
#include "App.h"
#include "LoggerMsg.h"
#include "SignalHandler.h"

namespace app {

	int App::run(int argc, char** argv) {
		// Load application configuration (to be implemented)
		//auto config = core::Config::load(argc, argv);

		// Log startup message
		logger.info(LoggerMsg::APP_START);

		// Main loop placeholder
		while(!core::SignalHandler::shutdown_requested()) {
		}

		// Log shutdown message
		logger.info(LoggerMsg::APP_TERMINATE);
		return 0;
	}

}
