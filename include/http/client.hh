#ifndef HTTP_CLIENT_HH
#define HTTP_CLIENT_HH

#include "http/request.hh"
#include "http/callbacks.hh"
#include "http/uri.hh"

#include <tcp/tcp.hh>

#include <string>
#include <functional>

namespace http
{
    struct client
    {
	static const uint16_t DEFAULT_PORT = 80;

	client(std::string const& hostname,
	       uint16_t post = DEFAULT_PORT);

	void request(http::request const& req,
		on_response_cb on_response);

    private:
	tcp::util::address addr;
    };
}

#endif
