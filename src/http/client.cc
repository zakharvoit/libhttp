#include "http/callbacks.hh"
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
		     on_response_cb on_response)
{
	// TODO: Remove this, when feature will be implemented
	if (has_sender) {
		throw runtime_error("Client already has sender (Now simultanious senders are not implemented).");
	}
	// HACK: Placement new used because of possible glibcxx bug
	// in string move operator, this method should be called only once
	// or some leaks will be possible
	new (&sender) impl::request_sender(req, addr, on_response);
	has_sender = true;
}

client::~client()
{
	// TODO: Look in the client::request, the same problem should be here
	if (has_sender) {
		sender.~request_sender();
	}
}
