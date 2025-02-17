set(HEADERS
	app/App.h
	logger/ILogger.h
	logger/Logger.h
	logger/LoggerMsg.h
	network_handler/NetworkHandler.h)

set(SOURCES
	app/App.cpp
	logger/Logger.cpp
	main/main.cpp
	network_handler/NetworkHandler.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE app logger main network_handler)
