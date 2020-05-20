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

#include <mutex>
#include <stdexcept>
#include "WsServer.h"

namespace Fs2a {

	size_t WsServer::count()
	{
		std::unique_lock<std::mutex> lck(mux_a);
		return connections_a.size();
	}

	bool WsServer::init(boost::asio::io_service * iosvc_i)
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

		// Ugly hack to prevent diamond problem, duplicate code in WsClient
		set_close_handler(bind(&Fs2a::WsServer::on_close,this,_1));
		set_fail_handler(bind(&Fs2a::WsServer::on_fail,this,_1));
		set_http_handler(bind(&Fs2a::WsServer::on_http,this,_1));
		set_interrupt_handler(bind(&Fs2a::WsServer::on_interrupt,this,_1));
		set_message_handler(bind(&Fs2a::WsServer::on_msg,this,_1,_2));
		set_open_handler(bind(&Fs2a::WsServer::on_open,this,_1));
		set_ping_handler(bind(&Fs2a::WsServer::on_ping,this,_1,_2));
		set_pong_handler(bind(&Fs2a::WsServer::on_pong,this,_1,_2));
		set_pong_timeout_handler(bind(&Fs2a::WsServer::on_pong_timeout,this,_1,_2));
		set_validate_handler(bind(&Fs2a::WsServer::on_validate,this,_1));

		return true;
	}

	void WsServer::listen(const std::string & ip_i, const uint16_t port_i)
	{
		websocketpp::lib::error_code ec;

		websocketpp::server<asioNoLog>::listen(
			boost::asio::ip::address::from_string(ip_i), port_i, ec
		);
		LCEA(!ec, throw std::runtime_error("Failed listening on designated IP and port"),
		"Failed listening on IP %s at port %d: %s", ip_i.c_str(), port_i, ec.message().c_str());

		start_accept();
	}

	void WsServer::on_close(websocketpp::connection_hdl hdl_i)
	{
		std::unique_lock<std::mutex> lck(mux_a);
		connections_a.erase(hdl_i);
	}

	void WsServer::on_open(websocketpp::connection_hdl hdl_i)
	{
		std::unique_lock<std::mutex> lck(mux_a);
		connections_a.insert(hdl_i);
	}

} // Fs2a namespace
