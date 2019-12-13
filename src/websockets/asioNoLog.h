// vim:set ts=4 sw=4 noexpandtab:

#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/logger/stub.hpp>

/** Define our custom Websocket++ configuration struct for use in WsClient
 * and WsServer classes. */

namespace Fs2a {

	struct asioNoLog : public websocketpp::config::asio {
		typedef asioNoLog type;
		typedef websocketpp::config::asio base;
		typedef base::concurrency_type concurrency_type;
		typedef base::request_type request_type;
		typedef base::response_type response_type;
		typedef base::message_type message_type;
		typedef base::con_msg_manager_type con_msg_manager_type;
		typedef base::endpoint_msg_manager_type endpoint_msg_manager_type;

		// Stop all logging from websocketpp
		typedef websocketpp::log::stub alog_type;
		typedef websocketpp::log::stub elog_type;

		typedef base::rng_type rng_type;
		struct transport_config : public base::transport_config {
			typedef type::concurrency_type concurrency_type;
			typedef type::alog_type alog_type;
			typedef type::elog_type elog_type;
			typedef type::request_type request_type;
			typedef type::response_type response_type;
			typedef websocketpp::transport::asio::basic_socket::endpoint
				socket_type;
		};  

		typedef websocketpp::transport::asio::endpoint<transport_config>
			transport_type;
	};

} // Fs2a namespace
