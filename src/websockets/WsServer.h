/* Copyright (c) 2025 Bren de Hartog <bren@fs2a.pro>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

vim:set ts=4 sw=4 noexpandtab: */

#pragma once

#include <set>
#include <boost/asio/io_service.hpp>
#include <websocketpp/server.hpp>
#include "asioNoLog.h"
#include "Logger.h"

class wssrvclntCheck;

namespace Fs2a {

	/** Websocket Server base class.
	 * Subclass this class for your application and override the on_msg()
	 * callback method. The procedure should be:
	 *
	 * 1. Construct and configure the Fs2a Logger
	 * 2. Construct the IOctxtWrapper
	 * 3. Set stopWhenIdle() to false in the IOctxtWrapper
	 * 4. Initialize the subclass through the init() method and pass the
	 * io_context from the IOctxtWrapper
	 * 5. Call the listen() method with an IP and a port
	 * 6. Start the IOctxtWrapper either through runHere() or runThreads()
	 * 7. Connections are handled, messages are forwarded to your on_msg()
	 * method
	 * 8. When terminating, call stop_listening() on your subclass
	 * 9. stop the IOctxtWrapper instance
	 * 10. Destroy your subclass instance
	 * 11. Stop the Fs2a Logger */
	class WsServer : public websocketpp::server<asioNoLog>
	{
		// Allow Check class to check internals
		friend class ::wssrvclntCheck;

		protected:
		/// Set of connection handles
		std::set<
			websocketpp::connection_hdl,
			std::owner_less<websocketpp::connection_hdl>
		> connections_a;

		/// Internal mutex for connections_a set
		std::mutex mux_a;

		public:
		/// Constructor
		inline WsServer() :
		websocketpp::server<asioNoLog>()
		{ }

		/// Destructor
		~WsServer() = default;

		/** Count the number of open connections.
		 * @returns The number of connections. */
		size_t count();

		/** Initialize the I/O service.
		 * @param iosvc_i A valid pointer to a boost I/O service object.
		 * @returns True when successful, false if not. */
		bool init(boost::asio::io_service * iosvc_i);

		/** Listen on an IP and a port for new connections.
		 * The internal state variable is not updated to LISTENING, since
		 * any underlying state inconsistencies will be reported anyway.
		 * Otherwise I'd have to override all the listen() methods from
		 * websocketpp::server. */
		virtual void listen(const std::string & ip_i, const uint16_t port_i);

		/** Handle connection close. */
		virtual void on_close(websocketpp::connection_hdl hdl_i);

		/** Handle new connection. */
		virtual void on_open(websocketpp::connection_hdl hdl_i);

#include "WsBoilerplate.h"

	};

} // Fs2a namespace
