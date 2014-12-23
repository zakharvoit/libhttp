#ifndef HTTP_REQUEST_SENDER_HH
#define HTTP_REQUEST_SENDER_HH

#include "http/request.hh"
#include "http/response.hh"
#include "http/callbacks.hh"

#include <tcp/tcp.hh>

namespace http
{
    namespace impl
    {
	struct request_sender
	{
	    request_sender(request const&,
			   tcp::util::address const&,
			   on_response_cb const&);

	private:
	    void on_connect(request const&);
	    void on_wrote();
	    void on_read(tcp::util::maybe <tcp::util::buffer>&&);
	    
	    response::builder builder;
	    on_response_cb on_response;
	    tcp::async::client client;
	};
    }
}

#endif
