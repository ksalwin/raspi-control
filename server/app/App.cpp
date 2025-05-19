#include "App.h"
#include "LoggerMsg.h"
#include "SignalHandler.h"
#include "UdpDiscoveryServer.h"

namespace app {

	int App::run(int argc, char** argv) {
		// Initial log message to indicate server is starting
		logger.info(LoggerMsg::APP_START);

		// Load application configuration (to be implemented)
		//auto config = core::Config::load(argc, argv);


		// Create UDP Discovery Server and bind to shared io_context
		network::UdpDiscoveryServer discovery_server(io_context, DISCOVERY_PORT);

		// Define a callback if UDP broadcast received
		// [&] to capture log by reference
		discovery_server.on_discovery([&](const std::string client_ip, uint16_t port) {
			// Log IP and port of the client that sent server discovery
			std::string msg {"Discovery from: " + client_ip + ":" + std::to_string(port) +
							 ", received on server port: " + std::to_string(DISCOVERY_PORT)};
			logger.info(msg);
		});

		// Start discovery server (begins async listening for UDP messages)
		discovery_server.start();

		// Main loop: continues running until shutdown signal (SIGINT/SIGTERM) is received
		while(!core::SignalHandler::shutdown_requested()) {
			// Run all pending network or timer event for up to 100 ms
			io_context.run_for(std::chrono::milliseconds{100});
		}

		logger.info("Shutdown requested - stopping discovery");

		discovery_server.stop();
		io_context.restart();		// Reset internal state of io_context

		// Log shutdown message
		logger.info(LoggerMsg::APP_TERMINATE);
		return 0;
	}

}
