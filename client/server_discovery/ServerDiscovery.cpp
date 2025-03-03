#include "ServerDiscovery.h"

#include <iostream>

ServerDiscovery::ServerDiscovery() : KeepThreadAlive(false)
{
	std::cout << "ServerDiscovery: Constructor";
}

ServerDiscovery::~ServerDiscovery()
{
	std::cout << "ServerDiscovery: Destructor";
	stop();
}

void ServerDiscovery::run()
{
	for(int i = 0; i < 5; i++)
	{
		std::cout << "serverDiscovery: " << i << '\n';
	}
}

void ServerDiscovery::start()
{
	if(KeepThreadAlive.load() == false)
	{
		KeepThreadAlive = true;
		ServerDiscoveryThread = std::thread(&ServerDiscovery::run, this);
	}
}

void ServerDiscovery::stop()
{
	if(KeepThreadAlive == true)
	{
		KeepThreadAlive = false;

		if(ServerDiscoveryThread.joinable())
			ServerDiscoveryThread.join();
	}
}
