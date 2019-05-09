// vim:set ts=4 sw=4 noexpandtab:

#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <stdexcept>

namespace Fs2a {

	class Semaphore
	{
		private:
		/// Internal condition variable
		std::condition_variable convar_a;

		/// Internal counter
		uint16_t count_a;

		/// Flag to indicate destruction
		std::atomic<bool> destructing_a;

		/// Internal mutex
		std::mutex mux_a;

		public:
		/// Constructor
		Semaphore(uint16_t count_i = 0);

		/// Destructor
		~Semaphore();

		/** Return the current internal semaphore counter.
		 * @returns The internal counter. */
		uint16_t count();

		/** Decrease the internal semaphore counter, blocking if count is zero.
		 */
		void dec();

		/** Increase the internal semaphore counter and notify a waiting thread,
		 * if any. */
		void inc();

		/** Try to decrease the internal semaphore counter, but do not block
		 * if it's count is currently zero.
		 * @returns True if counter was decreased, false if not (and thus would
		 * have blocked otherwise). */
		bool try_dec();

	};

} // Fs2a namespace
