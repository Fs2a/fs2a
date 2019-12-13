// vim:set ts=4 sw=4 noexpandtab:

#include "WsClient.h"

namespace Fs2a {

	bool WsClient::connect(const std::string & uri_i)
	{
		websocketpp::lib::error_code ec;

		con_a = get_connection(uri_i, ec);
		if (ec) {
			LE("Connection to invalid URI \"%s\" requested: %s",
				uri_i.c_str(),
				ec.message().c_str()
			);
			con_a.reset();
			return false;
		}

		websocketpp::client<asioNoLog>::connect(con_a);
		return true;
	}

} // Fs2a namespace
