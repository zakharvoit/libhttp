#ifndef HTTP_RESPONSE_HH
#define HTTP_RESPONSE_HH

#include "http/impl/parser.hh"

#include <tcp/tcp.hh>

namespace http
{
	struct response
	{
		struct builder
		{
			builder& append(tcp::util::buffer const&);
			response create();

			bool finished() const
			{
				return is_finished;
			};

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

		std::vector <char> get_body() const
		{
			return body;
		}

		tcp::async::buffer to_buffer() const;

	private:
		status status;
		std::vector <std::pair <std::string, std::string> > headers;
		std::vector <char> body;
	};
}

#endif
