#ifndef HTTP_REQUEST_HH
#define HTTP_REQUEST_HH

#include "http/uri.hh"
#include "http/impl/parser.hh"

#include <tcp/tcp.hh>

#include <stdexcept>

namespace http
{
	struct request
	{
		enum method
		{
			GET,
			POST
		};

		struct builder
		{
			builder& append(tcp::util::buffer const&);
			request create();

			bool finished() const
			{
				return is_finished;
			};

		private:
			impl::parser::http_request_s repr;
			impl::parser::parser parse =
				impl::parser::http_request(repr);
			bool is_finished = false;

			friend request;
		};

		request() = default;
		request(method const& m, http::uri const& u)
			: method(m), uri(u) {}

		// TODO: Think a little about interface
		method get_method() const
		{
			return method;
		}

		tcp::util::buffer to_buffer() const;

	private:
		method method;
		http::uri uri;
	};
}

#endif
