#ifndef HTTP_RESPONSE_HH
#define HTTP_RESPONSE_HH

#include "http/impl/parser.hh"

#include <tcp/tcp.hh>

namespace http
{
	struct response
	{
		static const std::string DEFAULT_VERSION;

		enum status
		{
			OK = 200,
			NOT_FOUND = 404
		};

		struct builder
		{
			builder& append(tcp::util::buffer const&);
			response create();

			builder& set_version(std::string const&);
			builder& set_status(status);
			builder& set_comment(std::string const&);
			builder& add_header(std::string const& key,
			                    std::string const& value);
			builder& set_text(std::vector <char> const& chars);
			builder& set_text(std::string const& s);
			builder& append_text(std::string const& s);

			bool finished() const
			{
				return is_finished;
			};

		private:
			impl::parser::http_response_s repr = {
				.version = DEFAULT_VERSION,
				.status = OK,
				.status_description = "OK"
			};
			impl::parser::parser parse =
				impl::parser::http_response(repr);
			bool is_finished = false;

			friend response;
		};
	
		status get_status() const
		{
			return status;
		}

		std::vector <char> get_body() const
		{
			return body;
		}

		std::string as_string() const
		{
			return std::string(body.begin(), body.end());
		}

		tcp::async::buffer to_buffer() const;

	private:
		status status;
		std::string comment;
		std::vector <std::pair <std::string, std::string> > headers;
		std::vector <char> body;
	};
}

#endif
