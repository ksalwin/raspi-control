#pragma once

#include <boost/asio/io_context.hpp>
#include <string>

#include "ILogger.h"

namespace app {

	class App {
		public:
			App(boost::asio::io_context & io_context_, ILogger & logger)
				: logger(logger), io_context {io_context} {}
			int run(int argc, char** argv);
		private:
			boost::asio::io_context & io_context;
			ILogger & logger;
	};
}
