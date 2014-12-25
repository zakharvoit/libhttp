#ifndef HTTP_PEER_HH
#define HTTP_PEER_HH

#include "http/response.hh"

#include <tcp/tcp.hh>

#include <functional>

namespace http
{
	struct peer
	{
		peer(tcp::async::client&);

		void send(http::response const&, // TODO: Pass maybe as arg
		          std::function<void()> const& callback = [](){});
		     
	private:
		tcp::async::client& tcp_client;
	};
}

#endif
