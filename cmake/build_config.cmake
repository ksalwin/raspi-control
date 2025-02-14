set(HEADERS
	app/App.h
	logger/ILogger.h
	logger/Logger.h
	logger/LoggerMsg.h
	network_handler/NetworkHandler.h)

set(SOURCES
	app/App.cpp
	logger/Logger.cpp
	network_handler/NetworkHandler.cpp
	src_main/main.cpp)

add_executable(raspi-control ${HEADERS} ${SOURCES})

target_include_directories(raspi-control PRIVATE app logger network_handler src_main)
