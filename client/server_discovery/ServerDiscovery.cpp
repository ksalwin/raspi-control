#include "ServerDiscovery.h"

#include <chrono>
#include <cstring>
#include <iostream>

ServerDiscovery::ServerDiscovery() : keep_thread_alive(false), sockfd(-1)
{
	std::cout << "[" << class_name << "] " << "Constructor\n";
	setup_socket();
}

ServerDiscovery::~ServerDiscovery()
{
	std::cout << "[" << class_name << "] " << "Destructor\n";
	stop();
	close_socket();
}

//--- Private ------------------------------------------------------------------

void ServerDiscovery::run()
{
	ssize_t sendto_result;

	while(keep_thread_alive.load() == true)
	{
		// Send broadcast message
		sendto_result = sendto(	sockfd,
								broadcast_msg,
								strlen(broadcast_msg),
								0,
								(struct sockaddr*)&broadcast_addr,
								sizeof(broadcast_addr));
		if(sendto_result < 0)
			std::cout << "[" << class_name << "] " << "Failed to send broadcast msg\n";
		else
			std::cout << "[" << class_name << "] " << "Broadcast msg sent\n";

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::cout << "[" << class_name << "] " << "Exiting run thread\n";
}

void ServerDiscovery::setup_socket()
{
	int setsockopt_result;

	// Create a UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		std::cout << "[" << class_name << "] " << "ERROR: Socker creation failed\n";
		return;
	}

	// Enable broadcast permission on socket
	setsockopt_result = setsockopt(	sockfd,
									SOL_SOCKET,
									SO_BROADCAST,
									&broadcast_enable,
									sizeof(broadcast_enable));
	if(setsockopt_result < 0)
	{
		std::cout << "[" << class_name << "] " << "ERROR: Failed to enable broadcast\n";
		close(sockfd);
		return;
	}

	// Configure broadcast address
	memset(&broadcast_addr, 0, sizeof(broadcast_addr));
	broadcast_addr.sin_family		= AF_INET;
	broadcast_addr.sin_port			= htons(port);
	broadcast_addr.sin_addr.s_addr	= inet_addr(broadcast_ip);
}

void ServerDiscovery::close_socket(void)
{
	if(sockfd >= 0)
		close(sockfd);
}

//--- Public -------------------------------------------------------------------

void ServerDiscovery::start()
{
	if(keep_thread_alive.load() == false)
	{
		keep_thread_alive = true;
		server_discovery_thread = std::thread(&ServerDiscovery::run, this);
		std::cout << "[" << class_name << "] " << "Thread start\n";
	}
}

void ServerDiscovery::stop()
{
	if(keep_thread_alive.load() == true)
	{
		keep_thread_alive = false;

		if(server_discovery_thread.joinable())
		{
			server_discovery_thread.join();
			std::cout << "[" << class_name << "] " << "Thread stopped\n";
		}
	}
}
