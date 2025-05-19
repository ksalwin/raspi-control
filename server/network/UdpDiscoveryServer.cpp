#include "UdpDiscoveryServer.h"

#include <boost/asio.hpp>
#include <iostream>

namespace network {
	// Predefined symbols
	namespace {
		constexpr char ExpectedPhrase[] = "raspi-control server are you there?";
		constexpr std::size_t MaxDatagram = 1024;
	}

	// Import the 'udp' type alias from Boost.Asio for brevity.
	// This allows to write 'udp::socket' instead of 'boost::asio::ip::udp::socket'.
	using boost::asio::ip::udp;

	// Internal implementation class that manages the UDP socket and handles asynchronous
	// receiving.
	class UdpDiscoveryServer::Impl {
		public:
			// Constructor: bind the socket to the specified port using IPv4
			Impl(boost::asio::io_context& io_context, uint16_t port)
				: socket(io_context, udp::endpoint(udp::v4(), port)),
				  buffer_(MaxDatagram)
			{
				// Allow quick restart & receive of broadcast frames
				// If server stops and restarts quickly (e.g., during development or
				// crashes), the port might still be marked as "in use" by the OS.
				// Without this line:
				// - An error may happen: "Address already in use"
				// - Waiting several seconds before running the server again is required
				// With this line:
				// - The server can immediately re-bind to the same port, no waiting
				socket.set_option(boost::asio::socket_base::reuse_address(true));
				// Enable reception of broadcast packets on this socket
				socket.set_option(boost::asio::socket_base::broadcast(true));
			}

			// Start asynchronous receiving of UDP packets
			void start() {
				running = true;
				receive();
			}
			// Stop listening and close the socket
			void stop() {
				running = false;
				boost::system::error_code ec;
				socket.cancel(ec);
				socket.close(ec);
			}
			bool is_running() const { return running; }
			// Set the callback to be invoked when a discovery message is received
			void set_callback(DiscoveryCallback_t cb) {
				// Store the user-provided callback so it can be invoked later.
				// std::move is used here to avoid copying large state inside std::func.
				// After moving, 'cb' should not be used again.
				callback_ = std::move(cb);
			}

	private:
		// Begin an asynchronous receive operation for incoming UDP datagrams.
		// This method sets up a non-blocking read that triggers a callback when data arrives.
		void receive() {
			socket.async_receive_from(
				boost::asio::buffer(buffer_),	// Destination buffer for incoming data
				remote_endpoint_,	// Endpoint to store sender's address and port
				[this](boost::system::error_code ec, std::size_t bytes_received) {
					// Lambda function to be called by Boost.Asio when UDP message received.
					// 'ec' reports the status of the operatio
					// 'bytes_received' tells how many bytes received


					// Check if receive was successful and data is present
					if (!ec && bytes_received > 0) {
						check(bytes_received);
					} else if (ec != boost::asio::error::operation_aborted) {
						std::cerr << "UDP receive error: " << ec.message() << "\n";
					}
					// Set up another asynchronous receive to keep listening for new
					// discovery messages.Boost.Asio's async operations are one-shot, so
					// we must call fo_receive() again. This keeps the server alive and
					// ready to handle the next incoming UDP packet.
					if (running) receive();
			});
		}

		void check(std::size_t bytes_received) {
			// Check payload size
			// Check message (char [])
			if ((bytes_received == std::strlen(ExpectedPhrase)) &&
				(std::memcmp(buffer_.data(), ExpectedPhrase, bytes_received) == 0))
			{
				// Check if callback was set and call it
				if (callback_)
				{
					callback_(	remote_endpoint_.address().to_string(),
								remote_endpoint_.port());
				}
			}

			// Q: Why not to check broadcast addr?
			// Boost.Asio (and most OSes) does not expose the destination address of
			// incoming UDP packets unless enabled platform-specific options
			// (like IP_PKTINFO on Linux).
			// Server receives client's IP in remote_endpoint.address(), not the IP that
			// the packet was sent to.
		}

		bool running{false};			// Flag indicating running server
		udp::socket socket;				// UDP socket for receiving broadcast messages
		udp::endpoint remote_endpoint_;	// Endpoint representing the sender of the last received packet
		std::vector<char> buffer_;		// Buffer to hold incoming message data
		UdpDiscoveryServer::DiscoveryCallback_t callback_; // Callback function to notify on discovery
	};

// Public interface implementation

	// Constructor: initialize implementation with I/O context and port
	UdpDiscoveryServer::UdpDiscoveryServer(	boost::asio::io_context & io_context,
											uint16_t port)
		: impl_(std::make_unique<Impl>(io_context, port)) {}

	// Destructor: clean up the implementation instance
	UdpDiscoveryServer::~UdpDiscoveryServer() = default;

	void UdpDiscoveryServer::start()			{ impl_->start();				}
	void UdpDiscoveryServer::stop()				{ impl_->stop();				}
	bool UdpDiscoveryServer::is_running() const	{ return impl_->is_running();	}
	void UdpDiscoveryServer::on_discovery(DiscoveryCallback_t cb)
												{ impl_->set_callback(std::move(cb));}
}
