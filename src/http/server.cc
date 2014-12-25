#include "http/server.hh"
#include "http/main_loop.hh"

using namespace tcp;
using namespace http;
using namespace std;

server::server(string const& addr,
               on_request_cb const& on_request,
               uint16_t port)
	: tcp_server(util::address(addr + ":" + to_string(port)))
	, on_request(on_request)
	, listening(tcp_server.listen(main_loop::get_service(),
	                              [=](auto arg) { this->on_accept(std::move(arg)); }))
{
	
}

void server::on_accept(util::maybe<async::client>&& cm)
{
	cm.raise(); // TODO: Handle errors better
	tcp_client = cm.get();
	tcp_client.read_some(main_loop::get_service(), [=](auto arg)
	                     {
		                     this->on_read_some(std::move(arg));
	                     });
}

void server::on_read_some(util::maybe<util::buffer>&& bm)
{
	bm.raise(); // TODO: Handle errors better
	builder.append(bm.get());
	if (builder.finished()) {
		on_request(builder.create(), peer(tcp_client));
		// TODO: Here we should clear builder, also check client.cc code
	} else {
		tcp_client.read_some(main_loop::get_service(), [=](auto arg)
		                     {
			                     this->on_read_some(std::move(arg));
		                     });
	}
}
