#pragma once

#include <atomic>
#include <thread>

class ServerDiscovery {
	private:
		std::atomic<bool> KeepThreadAlive;
		std::thread ServerDiscoveryThread;
		void run();
	public:
		ServerDiscovery();
		~ServerDiscovery();
		void start();
		void stop();
};
