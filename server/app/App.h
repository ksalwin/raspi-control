#pragma once

#include <string>
#include "ILogger.h"

namespace app {

	class App {
		public:
			App(ILogger& _logger) : logger(_logger) {}
			int run(int argc, char** argv);
		private:
			ILogger& logger;
	};
}
