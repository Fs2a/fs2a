// vim:set ts=4 sw=4 noexpandtab:

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
	 * 2. Construct the IOsvcWrapper
	 * 3. Set stopWhenIdle() to false in the IOsvcWrapper
	 * 4. Initialize the subclass through the init() method and pass the
	 * io_service from the IOsvcWrapper
	 * 5. Call the listen() method with an IP and a port
	 * 6. Start the IOsvcWrapper either through runHere() or runThread()
	 * 7. Connections are handled, messages are forwarded to your on_msg()
	 * method
	 * 8. When terminating, call stop_listening() on your subclass
	 * 9. stop the IOsvcWrapper instance
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
