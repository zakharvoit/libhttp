#include "http/peer.hh"
#include "http/main_loop.hh"

#include <memory>

using namespace http;
using namespace tcp;
using namespace std;

peer::peer(async::client&& client)

	: tcp_client(std::move(client)) {}

void peer::send(http::response const& response,
                function<void()> const& callback)
{
	// This will ensure that tcp_client is alive until callback will
	// be called.
	auto ptr = new async::client(std::move(tcp_client));
	ptr->write(main_loop::get_service(),
	                 response.to_buffer(),
	                 [callback, ptr](auto r)
	                 {
		                 delete ptr;
		                 r.raise(); // TODO: Handle errors better
		                 callback();
	                 });
}
