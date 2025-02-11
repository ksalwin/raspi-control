#include <iostream>

#include "logger.h"

int main() {
	Logger logger;

	logger.write(LoggerMsg::APP_START);

	std::cout << "Hello world!" << '\n';
	
	logger.write(LoggerMsg::APP_TERMINATE);
}
