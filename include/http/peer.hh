#ifndef HTTP_PEER_HH
#define HTTP_PEER_HH

#include "http/response.hh"
#include "http/request.hh"

#include <tcp/tcp.hh>

#include <functional>

namespace http
{
	struct peer
	{
		peer(tcp::async::client&&,
		     http::request const& request);

		void send(http::response const&,
		          std::function<void(tcp::util::maybe<tcp::util::nothing>)> const& callback = [](auto){});

		http::request get_request() const
		{
			return request;
		}
		     
	private:
		tcp::async::client tcp_client;
		http::request request;
	};
}

#endif
