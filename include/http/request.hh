#ifndef HTTP_REQUEST_HH
#define HTTP_REQUEST_HH

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
			builder& set_method(method);
			builder& set_uri(std::string const&);
			builder& add_header(std::string const& key,
			                    std::string const& value);
			builder& set_text(std::vector <char> const& body);
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
		request(method const& m, std::string const& uri)
			: method(m), uri(uri) {}

		tcp::util::buffer to_buffer() const;

		method get_method() const
		{
			return method;
		}

		auto get_headers() const
		{
			return headers;
		}

		auto get_uri() const
		{
			return uri;
		}

		auto get_text() const
		{
			return body;
		}

		auto get_text_as_string() const
		{
			return std::string(body.begin(), body.end());
		}

	private:
		method method;
		std::vector <std::pair <std::string, std::string>> headers;
		std::string uri;
		std::vector <char> body;
	};
}

#endif
