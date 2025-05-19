#pragma once

#include <boost/asio/io_context.hpp>
#include <string>

#include "ILogger.h"

namespace app {

	class App {
		public:
			// Port on which this server will listen for UDP broadcast discovery messages
			static constexpr uint16_t DISCOVERY_PORT{12345};

			App(boost::asio::io_context & io_context, ILogger & logger)
				: io_context (io_context), logger(logger) {}

			int run(int argc, char** argv);
		private:
			boost::asio::io_context & io_context;
			ILogger & logger;
	};

}
