// vim:set ts=4 sw=4 noexpandtab:

#pragma once

#include <string>
#include <boost/asio/io_service.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_client.hpp>
#include "asioTlsClientNoLog.h"
#include "../Logger.h"

#define WSTYPE websocketpp::config::asio_tls_client

namespace Fs2a {

	class WsClient : public websocketpp::client<WSTYPE>
	{
		public:
		/// Constructor
		inline WsClient() :
		websocketpp::client<WSTYPE>()
		{ }

		/// Destructor
		virtual ~WsClient() = default;

		/// Shared pointer to connection object
		connection_ptr con_a;

		/** Initialize the I/O service.
		 * @param iosvc_i A valid pointer to a boost I/O service object.
		 * @returns True when successful, false if not. */
		inline bool init(boost::asio::io_service * iosvc_i)
		{
			websocketpp::lib::error_code ec;

			LCWA(iosvc_i != nullptr, return false,
				"Boost I/O service pointer for initialization is NULL");
			init_asio(iosvc_i, ec);
			LCEA(!ec, return false,
				"Asynchronous I/O initialization failed: %s", ec.message().c_str());

			using websocketpp::lib::placeholders::_1;
			using websocketpp::lib::placeholders::_2;
			using websocketpp::lib::bind;

			// Set the TLS init handler
			set_tls_init_handler(Fs2a::WsClient::tlsInitHandler);

			// Ugly hack to prevent diamond problem, duplicate code in WsClient
			set_close_handler(bind(&Fs2a::WsClient::on_close,this,_1));
			set_fail_handler(bind(&Fs2a::WsClient::on_fail,this,_1));
			set_http_handler(bind(&Fs2a::WsClient::on_http,this,_1));
			set_interrupt_handler(bind(&Fs2a::WsClient::on_interrupt,this,_1));
			set_message_handler(bind(&Fs2a::WsClient::on_msg,this,_1,_2));
			set_open_handler(bind(&Fs2a::WsClient::on_open,this,_1));
			set_ping_handler(bind(&Fs2a::WsClient::on_ping,this,_1,_2));
			set_pong_handler(bind(&Fs2a::WsClient::on_pong,this,_1,_2));
			set_pong_timeout_handler(bind(&Fs2a::WsClient::on_pong_timeout,this,_1,_2));
			set_validate_handler(bind(&Fs2a::WsClient::on_validate,this,_1));

			return true;
		}

		/** Connect to a ws:// or ws:/// URI.
		 * @param uri_i Full URI to connect to.
		 * @returns True if URI was valid, false if not. A return value
		 * of true does *NOT* indicate a successful connection! */
		virtual bool connect(const std::string & uri_i);

#include "WsBoilerplate.h"

		/** @{ Specify explicitly, because it can't be included in the
		 * Boilerplate header, because they need to be implemented
		 * differently in the WsServer class. */
		virtual inline void on_close(websocketpp::connection_hdl hdl_i) { UNUSED(hdl_i); }
		virtual inline void on_open(websocketpp::connection_hdl hdl_i) { UNUSED(hdl_i); }
		/** @} */

		static inline websocketpp::lib::shared_ptr<boost::asio::ssl::context>
			tlsInitHandler(websocketpp::connection_hdl hdl_i)
		{
			UNUSED(hdl_i);

			websocketpp::lib::shared_ptr<boost::asio::ssl::context> sp;
			sp.reset(new boost::asio::ssl::context(boost::asio::ssl::context::sslv23));
			sp->set_default_verify_paths();

			return sp;
		}

	};

} // Fs2a namespace
