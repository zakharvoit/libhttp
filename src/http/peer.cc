#include "http/peer.hh"
#include "http/main_loop.hh"

#include <memory>

using namespace http;
using namespace tcp;
using namespace std;

peer::peer(async::client&& client,
           http::request const& request)

	: tcp_client(std::move(client)),
	  request(request)
{}

void peer::send(http::response const& response,
                function<void(tcp::util::maybe<tcp::util::nothing>)> const& callback)
{
	// This will ensure that tcp_client is alive until callback will
	// be called.
	// TODO: There are possible leaks, try std::{shared,unique}_ptr
	auto ptr = new async::client(std::move(tcp_client));
	ptr->write(main_loop::get_service(),
	                 response.to_buffer(),
	                 [callback, ptr](auto r)
	                 {
		                 delete ptr;
		                 callback(r);
	                 });
}
