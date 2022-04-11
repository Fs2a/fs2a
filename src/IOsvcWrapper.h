// vim:set ts=4 sw=4 noet:
/*
Copyright (c) 2022, Fs2a, Bren de Hartog <bren@fs2a.pro>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#pragma once

#include <memory>
#include <thread>
#include <vector>
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
