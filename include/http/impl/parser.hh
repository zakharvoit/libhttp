#ifndef HTTP_IMPL_PARSER_HH
#define HTTP_IMPL_PARSER_HH

#include <tcp/tcp.hh>

#include <functional>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>

namespace http
{
	namespace impl
	{
		namespace parser
		{
			typedef std::function<bool(tcp::util::buffer&)> parser;
#define P(buf, args...) [args](tcp::util::buffer& buf) mutable -> bool

			struct http_header_s
			{
				std::string key;
				std::string value;

				bool operator==(http_header_s const&) const;
			};

			struct http_response_s
			{
				std::string version;
				int status = 0;
				std::string status_description;
				std::vector <http_header_s> headers;
				std::vector <char> body;
				size_t body_length = 0;

				bool operator==(http_response_s const&) const;
			};

			struct http_request_s
			{
				std::string method;
				std::string uri;
				std::string version;
				std::vector <http_header_s> headers;

				bool operator==(http_request_s const&) const;
			};

			struct parse_error : std::runtime_error
			{
				parse_error(std::string const& s)
					: runtime_error(s) {}
			};

			parser operator>>(parser a, parser b);
			parser operator|(parser a, parser b);
			parser endline();
			parser until_endl(std::string& result);
			parser token(std::string& result);
			parser http_header(http_header_s& header);
			parser http_headers(std::vector <http_header_s>& headers,
			                    size_t& content_length);
			parser http_body(char*& body,
			                 size_t& length);
			parser http_response(http_response_s& response);

			parser http_request(http_request_s& request);

			template <char C>
			parser character()
			{
				bool ended = false;
				return P(buf, =) {
					if (ended) return true;
					if (buf.rest_length()) {
						if (**buf == C) {
							buf += 1;
							return ended = true;
						} else {
							ended = true;
							throw parse_error(std::string("Expected ") + C
							                  + ", but found " + **buf);
						}
					}
					return false;
				};
			}

			template <typename Int>
			Int to_int(std::string s)
			{
				Int res = 0;
				for (auto c : s) {
					res *= 10;
					res += c - '0';
				}
				return res;
			}

			template <typename A, typename B>
			std::function<parser(A&)> fmap(std::function<A(B)> f,
			                               std::function<parser(B&)> p)
			{
				return [=](A& a) mutable {
					bool start = true;
					B b;
					auto pp = p(b);
					return P(buf, =, &a) {
						if (start) {
							pp = p(b);
							start = false;
						}
						if (pp(buf)) {
							a = f(b);
							return true;
						}
						return false;
					};
				};
			}
    
			template <typename Int>
			parser integer(Int& result)
			{
				return fmap(std::function<Int(std::string)>(to_int<Int>),
				            std::function<parser(std::string&)>(token))(result);
			}
		}
	}
}

#endif
