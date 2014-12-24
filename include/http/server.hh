#ifndef HTTP_SERVER_HH
#define HTTP_SERVER_HH

#include "http/callbacks.hh"
#include "http/request.hh"

#include <tcp/tcp.hh>

#include <string>

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
		void on_read_some(tcp::util::maybe<tcp::util::buffer>&&);

		tcp::async::server tcp_server;
		tcp::async::client tcp_client;
		on_request_cb on_request;
		request::builder builder;
		tcp::util::canceller listening;
	};
}

#endif
