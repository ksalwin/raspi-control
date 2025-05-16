#include "App.h"
#include "Logger.h" 
//#include <core/SignalHandler.h>

int main(int argc, char** argv) {
    // Set up signal handlers for clean shutdown on SIGINT/SIGTERM
    //core::SignalHandler::setup();

    // Create a concrete logger that implements ILogger
    Logger logger;

    // Inject the logger into App
    app::App app(logger);

    // Run the application
    return app.run(argc, argv);
}

