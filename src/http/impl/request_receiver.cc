#include "http/impl/request_receiver.hh"
#include "http/main_loop.hh"
#include "http/peer.hh"

using namespace tcp;
using namespace http;
using namespace std;

// Create fake receiver
// TODO: There are overhead of creating and destroying sockets
request_receiver::request_receiver(int id)
	: id(id)
{
	
}

request_receiver::request_receiver(async::client&& client,
                                   on_request_cb const& on_request,
                                   function<void(int)> on_finish)
	: client(std::move(client)),
	  on_request(on_request),
	  on_finish(on_finish)
{
	static int max_id = 0;
	this->id = max_id++;
	this->client.read_some(main_loop::get_service(), [=](auto arg)
	                 {
		                 on_read_some(std::move(arg));
	                 });
}

void request_receiver::on_read_some(util::maybe<util::buffer>&& bm)
{
	if (!bm) {
		on_request(bm.get_error());
		on_finish(id);
		return;
	}
	builder.append(bm.get());
	if (builder.finished()) {
		on_request(peer(std::move(client),
		                builder.create()));
		on_finish(id);
	} else {
		client.read_some(main_loop::get_service(), [=](auto arg)
		                 {
			                 on_read_some(std::move(arg));
		                 });
	}
}
