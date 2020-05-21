/* Copyright (c) 2020 Simon de Hartog <simon@dehartog.name>

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

#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/logger/stub.hpp>

/** Define our custom Websocket++ configuration struct for use in WsClient
 * and WsServer classes. */

namespace Fs2a {

	struct asioTlsClientNoLog : public websocketpp::config::asio_tls_client {
		typedef asioTlsClientNoLog type;
		typedef websocketpp::config::asio_tls_client base;
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
			typedef websocketpp::transport::asio::tls_socket::endpoint
				socket_type;
		};  

		typedef websocketpp::transport::asio::endpoint<transport_config>
			transport_type;
	};

} // Fs2a namespace
