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

/** {@ Callback handler methods. These should be overriden when a
 * subclass wants to actually handle the corresponding event.
 * @param hdl_i Websocket++ connection handle.
 * @param msg_i Pointer to received message.
 * @param text_i Text inside ping and pong message. */
virtual inline void on_fail(websocketpp::connection_hdl hdl_i) { UNUSED(hdl_i); }
virtual inline void on_http(websocketpp::connection_hdl hdl_i) { LD("WsServer::on_http ALSO EXECUTED!!!"); UNUSED(hdl_i); }
virtual inline void on_interrupt(websocketpp::connection_hdl hdl_i) { UNUSED(hdl_i); }
virtual inline void on_msg(
	websocketpp::connection_hdl hdl_i,
	message_ptr msg_i) {
	LD("Boilerplate ON_MSG!!!");
	UNUSED(hdl_i); UNUSED(msg_i);
}
virtual inline bool on_ping(websocketpp::connection_hdl hdl_i, std::string text_i) {
	UNUSED(hdl_i); UNUSED(text_i); return true;
}
virtual inline void on_pong(websocketpp::connection_hdl hdl_i, std::string text_i) {
	UNUSED(hdl_i); UNUSED(text_i);
}
virtual inline void on_pong_timeout(websocketpp::connection_hdl hdl_i, std::string text_i) {
	UNUSED(hdl_i); UNUSED(text_i);
}
virtual inline bool on_validate(websocketpp::connection_hdl hdl_i) {
	UNUSED(hdl_i); return true;
}
/** @} */
