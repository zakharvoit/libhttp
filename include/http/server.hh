#ifndef HTTP_SERVER_HH
#define HTTP_SERVER_HH

#include "http/callbacks.hh"
#include "http/request.hh"
#include "http/impl/request_receiver.hh"

#include <tcp/tcp.hh>

#include <string>
#include <set>
#include <functional>

namespace http
{
	struct server
	{
		static const uint16_t DEFAULT_PORT = 80;

		server(std::string const& hostname,
		       on_request_cb const&,
		       uint16_t port = DEFAULT_PORT);

	private:
		void on_accept(tcp::util::maybe<tcp::async::client>&&);

		tcp::async::server tcp_server;
		on_request_cb on_request;
		tcp::util::canceller listening;
		std::set <request_receiver> receivers;
	};
}

#endif
