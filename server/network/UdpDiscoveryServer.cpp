#include "UdpDiscoveryServer.h"

#include <boost/asio.hpp>
#include <iostream>

namespace network {
	// Predefined symbols
	namespace {
		constexpr char ExpectedPhrase[] = "raspi-control server are you there?";
		constexpr std::size_t MaxDatagram = 1024;
		const boost::asio::ip::address_v4 BroadcastAddr =
			boost::asio::ip::make_address_v4("192.168.0.255");
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
				: socket_(io_context, udp::endpoint(udp::v4(), port)),
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
        		socket_.set_option(boost::asio::socket_base::reuse_address(true));
				// Enable broadcast reception so this socket can receive UDP broadcast packets
				socket_.set_option(boost::asio::socket_base::broadcast(true));
			}

			// Start asynchronous receiving of UDP packets
			void start() {
				running_ = true;
				receive();
			}
			// Stop listening and close the socket
			void stop() {
				running_ = false;
				boost::system::error_code ec;
				socket_.cancel(ec);
				socket_.close(ec);
			}
			bool is_running() const { return running_; }
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
			socket_.async_receive_from(
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
					if (running_) receive();
			});
		}

		void check(std::size_t bytes_received) {
			// Check destination address
			// Check payload size
			// Check payload message
			// Check callback
			if ((remote_endpoint_.address() == BroadcastAddr) && 
				(bytes_received == std::strlen(ExpectedPhrase)) &&
				(buffer_.data() == ExpectedPhrase)) {

				if (callback_) {
					callback_(	remote_endpoint_.address().to_string(),
								remote_endpoint_.port());
				}
			}
        }
		
		bool running_ {false};
		udp::socket socket_;			// UDP socket for receiving broadcast messages
		udp::endpoint remote_endpoint_;	// Endpoint representing the sender of the last received packet
		std::vector<char> buffer_;		// Buffer to hold incoming message data
		UdpDiscoveryServer::DiscoveryCallback_t callback_; // Callback function to notify on discovery
	};

// Public interface implementation

	// Constructor: initialize implementation with I/O context and port
	UdpDiscoveryServer::UdpDiscoveryServer(boost::asio::io_context& io_context,
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
