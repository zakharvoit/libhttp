#include "http/client.hh"
#include "http/main_loop.hh"

using namespace std;
using namespace http;
using namespace tcp::util;

client::client(string const& hostname, uint16_t port)
    : addr(hostname + ":" + to_string(port))
{
}

void client::request(http::request const& req,
		     function <http::response> const& on_response)
{
    impl::request_sender(req, on_response);
}
