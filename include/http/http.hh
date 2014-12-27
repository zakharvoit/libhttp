#ifndef HTTP_HTTP_HH
#define HTTP_HTTP_HH

#include "http/client.hh"
#include "http/server.hh"
#include "http/main_loop.hh"

namespace http
{
	// Import maybe in our namespace

	using tcp::util::maybe;
	using tcp::util::nothing;
	using tcp::util::buffer;
}

#endif
