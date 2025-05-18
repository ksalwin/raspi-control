#pragma once
/**
 * UdpDiscoveryServer
 * ------------------
 * Listens for a specific UDP broadcast message and notifies the application
 * when it arrives.  One server instance ↔ one UDP port.
 *
 *  • Asynchronous, single-threaded (runs on the supplied asio::io_context).
 *  • Uses the Pimpl idiom to keep Boost.Asio headers out of public headers.
 *  • Filters on
 *        – destination address   : 192.168.0.255
 *        – payload (ASCII exact) : "raspi-control server are you there?"
 *
 * To use:
 *   network::UdpDiscoveryServer server(io, 12345);
 *   server.onDiscovery([](std::string ip, uint16_t port) { ... });
 *   server.start();
 */

#include <cstdint>		// Fixed-width types, like uint16_t
#include <functional>	// Needed for std::function, which is used in discovery callback
#include <memory>		// Needed for unique_ptr
#include <string>		// Needed for callback

// Forward declarations for Boost.Asio types to avoid including the full Asio headers in
// this file. This minimizes compile-time dependencies and improves build performance.
// These declarations are sufficient because we only need pointers or references to these
// types in this header; we do not access their internal members or require their full
// definitions here. No need to know internat structure.
// Including big headers like <boost/asio.hpp> can slow down compilation.
namespace boost{
	namespace asio {
		// Represents the core I/O service that drives asynchronous operations, including
		// networking and timers
		class io_context;
	}
}

namespace network {
	class UdpDiscoveryServer {
		public:
			// Type for a callback when a discovery message is received
			using DiscoveryCallback_t =
				std::function<void(const std::string& client_ip, uint16_t client_port)>;

			// Constructor: Server will create UDP socket and bind to specific port number
			// on all available network interfaces (Ethernet, Wi-Fi, loopback, etc.)
			UdpDiscoveryServer(boost::asio::io_context& io_context, uint16_t port);
			~UdpDiscoveryServer();

			void start();				// Start listening
			void stop();				// Stop  listening
			bool is_running() const;	// Query status
			// Register a function to be called when a discovery message is received.
			// This should be called by the user of the class (e.g., the App layer) before
			// calling start().
			// The callback provides the client's IP address and port number.
			void on_discovery(DiscoveryCallback_t cb); // Register callback


		private:
			// Forward declaration of implementation class (Pimpl Idiom) to:
			// - Hide Boost.Asio details and reduces compile-time dependencies.
			//   Hide implementation details from the public interface (keeps headers
			//   clean and decoupled)
			// - Minimize compile-time dependencies (e.g., no need to include Boost.Asio
			//   in the header)
			// - Improve binary compatibility — changes in Impl don’t require recompiling
			//   code that includes UdpDiscoveryServer.h
			// 
			// The actual definition and logic of Impl appears in the .cpp file.
			// This line is essential to encapsulate networking logic (socket, buffer,
			// callbacks) away from the interface.
			//
			// Implementation class containing private details (e.g., socket, buffer)
			class Impl;
			std::unique_ptr<Impl> impl_;      // Pointer to the implementation object
	};
}
