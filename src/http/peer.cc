#include "http/peer.hh"
#include "http/main_loop.hh"

using namespace http;
using namespace tcp;
using namespace std;

peer::peer(async::client& client)

	: tcp_client(client) {}

void peer::send(http::response const& response,
                function<void()> const& callback)
{
	tcp_client.write(main_loop::get_service(),
	                 response.to_buffer(),
	                 [=](auto r)
	                 {
		                 r.raise(); // TODO: Handle errors better
		                 callback();
	                 });
}
