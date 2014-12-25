#ifndef HTTP_CALLBACKS_HH
#define HTTP_CALLBACKS_HH

#include "http/request.hh"
#include "http/response.hh"
#include "http/peer.hh"

#include <functional>

namespace http
{
	typedef std::function <void(request, peer&&)> on_request_cb;
	typedef std::function <void(response)> on_response_cb;
}

#endif
