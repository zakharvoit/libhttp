#ifndef HTTP_RESPONSE_HH
#define HTTP_RESPONSE_HH

#include <tcp/tcp.hh>

#include "http/impl/parser.hh"

namespace http
{
    struct response
    {
	struct builder
	{
	    builder() = default;

	    builder& append(tcp::util::buffer const&);
	    response create();
	    bool finished() const;

	private:
	    impl::parser::http_response_s repr;
	    impl::parser::parser parse =
		impl::parser::http_response(repr);
	    bool is_finished = false;

	    friend response;
	};
	
	enum status
	{
	    OK = 200,
	    NOT_FOUND = 404
	};

	status get_status() const
	{
	    return status;
	}

    private:
	status status;
	std::vector <std::pair <std::string, std::string> > headers;
	std::vector <char> body;
    };
}

#endif
