#include <boost/asio/io_context.hpp>

#include "App.h"
#include "Logger.h" 
#include "SignalHandler.h"

int main(int argc, char** argv) {
	boost::asio::io_context io_context;	// Boost.Asio context; shared event loop
	Logger log;							// Logger instance


	core::SignalHandler::init();		// Set shutdown on SIGINT and SIGTERM signals


	app::App app(io_context, log);		// App instance

	return app.run(argc, argv);			// Run the App
}


