// vim:set ts=4 sw=4 noexpandtab:

#pragma once

#include <memory>
#include <thread>
#include <boost/asio/io_service.hpp>
#include "Singleton.h"

namespace Fs2a {

	class IOsvcWrapper : public Fs2a::Singleton<IOsvcWrapper>
	{
		/// Singleton template as friend for construction
		friend class Fs2a::Singleton<IOsvcWrapper>;

		private:
		/// Default constructor
		IOsvcWrapper();

		/// Copy constructor
		IOsvcWrapper(const IOsvcWrapper & obj_i) = delete;

		/// Assignment constructor
		IOsvcWrapper & operator=(const IOsvcWrapper & obj_i) = delete;

		/// Destructor
		~IOsvcWrapper();

		protected:
		/// Actual Boost I/O service object
		boost::asio::io_service iosvc_a;

		/// Thread to run I/O service in
		std::unique_ptr<std::thread> thread_a;

		/// Wrapped work object to keep io service running
		std::unique_ptr<boost::asio::io_service::work> work_a;

		public:
		/** Run the I/O service in place, blocking the calling thread. */
		void runHere();

		/** Run the I/O service in a separate thread. */
		void runThread();

		/** Check whether the I/O service would/will stop when it is idle.
		 * @returns True if it would/will stop, false if not. */
		inline bool stopWhenIdle() { return !work_a; }

		/** Define whether the I/O service should/will stop running in case
		 * there is no more work available.
		 * @param stopWhenIdle_i True if it should stop running when idle,
		 * false if it should keep running forever. */
		void stopWhenIdle(const bool stopWhenIdle_i);

		/** Stop the I/O service.
		 * Also terminates the thread if necessary. */
		void stop();

		/** Retrieve the actual io_service object for use in other objects.
		 * @returns reference to internal io_service object. */
		inline boost::asio::io_service & svc() { return this->iosvc_a; }

	};

} // Fs2a namespace
