// This class installs handlers for OS-level termination signals (SIGINT, SIGTERM)
// and provides a thread-safe way to check whether a shutdown has been requested.
// It enables graceful shutdown of long-running applications like servers or daemons.

#pragma once

#include <atomic>
#include <csignal>

namespace core {
	class SignalHandler {
		public:
			// Call once at program startup to install signal headers
			static void init() {
				std::signal(SIGINT,		handle_signal);
				std::signal(SIGTERM,	handle_signal);
			}

			// Check this flag periodically in your main loop
			static bool shutdown_requested() {
				return shutdown_requested_flag;
			}
		private:
			// Internal signal handler function
			static void handle_signal(int signum) {
				shutdown_requested_flag = true;
			}

			// Flag indicating a shutdown request was received
			//
			// That handler function (e.g., handleSignal) can be called:
			// - asynchronously
			// - from a different thread or even in the middle of a library call
			// - at any time when the signal is delivered
			//
			// So when the handler sets a value like this:
			// 			shutdown_requested = true;
			// then use an atomic to prevent data races.
			// Otherwise, the compiler or CPU could:
			// - reorder memory operations
			// - optimize out checks
			// - produce undefined behavior
			inline static std::atomic<bool> shutdown_requested_flag{false};
	};
}
