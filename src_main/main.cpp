#include "App.h"
#include "Logger.h"

int main() {
	Logger logger;

	App app(logger);
	app.start();

	return 0;
}
