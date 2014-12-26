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
	                              [=](auto arg)
	                              {
		                              this->on_accept(std::move(arg));
	                              }))
{
	
}

void server::on_accept(util::maybe<async::client>&& cm)
{
	if (!cm) {
		on_request(cm.get_error());
		return;
	}
	receivers.emplace(std::move(cm.get()),
	                  on_request,
	                  [=](int id)
	                  {
		                  receivers.erase({id});
	                  }
		);
}
