// vim:set ts=4 sw=4 noexpandtab:

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
