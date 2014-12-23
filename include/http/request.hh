#ifndef HTTP_REQUEST_HH
#define HTTP_REQUEST_HH

#include "http/uri.hh"

namespace http
{
    struct request
    {
	enum method
	{
	    GET,
	    POST
	};

	request(method method, http::uri uri)
	    : method(method), uri(uri) {}

	// TODO: Think a little about interface
	method get_method() const
	{
	    return method;
	}

    private:
	method method;
	http::uri uri;
    };
}

#endif
