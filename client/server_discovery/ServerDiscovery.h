#pragma once

#include <arpa/inet.h>
#include <atomic>
#include <netinet/in.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "Logger.h"

class ServerDiscovery {
	private:
		static constexpr const char *class_name {"ServerDiscovery"};

		Logger *const logger = nullptr;

		std::atomic<bool> keep_thread_alive;
		std::thread server_discovery_thread;

		// todo move to other network class
		int sockfd;
		struct sockaddr_in broadcast_addr;
		int broadcast_enable { 1 };
		const unsigned short int port { 12345 };
		static constexpr const char *broadcast_msg	{	"raspi-control server "
														"are you there?"		};
		static constexpr const char *broadcast_ip	{"192.168.0.255"};

		inline void log_if_possible(const std::string & msg);
		void run();
		void setup_socket();
		void close_socket();
	public:
		ServerDiscovery();
		ServerDiscovery(Logger *const logger);
		~ServerDiscovery();
		void start();
		void stop();
};

inline void ServerDiscovery::log_if_possible(const std::string & msg) {
	if(logger) logger->write(class_name + msg);
}
