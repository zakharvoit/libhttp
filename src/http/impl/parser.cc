#include "http/impl/parser.hh"

#include <functional>
#include <cstring>
#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>
using namespace std;

namespace http
{
	namespace impl
	{
		namespace parser
		{
			bool http_header_s::operator==(http_header_s const& r) const
			{
				return key == r.key
					&& value == r.value;
			}
	    
			bool http_response_s::operator==(http_response_s const& r) const
			{
				return version == r.version
					&& status == r.status
					&& status_description == r.status_description
					&& headers == r.headers
					&& body_length == r.body_length
					&& strncmp(body, r.body, body_length) == 0;
			}

			bool http_request_s::operator==(http_request_s const& r) const
			{
				return method == r.method
					&& uri == r.uri
					&& version == r.version
					&& headers == r.headers;
			}	
	    
			parser operator>>(parser a, parser b)
			{
				return P(buf, =) {
					return a(buf) && b(buf);
				};
			}

			parser operator |(parser a, parser b)
			{
				return P(buf, =) {
					try {
						return a(buf);
					} catch(parse_error const&) {
						return b(buf);
					}
				};
			}

			parser endline()
			{
				return character <'\n'>()
					| (character<'\r'>() >> character <'\n'>());
			}

			parser until_endl(string& result)
			{
				bool ended = false;
				bool wait_LF = false;
				return P(buf, =, &result) {
					if (ended) return true;
					if (!buf.rest_length()) return false;
					if (wait_LF) {
						if (**buf != '\n') {
							ended = true;
							throw parse_error("Expected endline, but"s
							                  + **buf + " found.");
						} else {
							buf += 1;
							return ended = true;
						}
					}
					while (buf.rest_length()
					       && **buf != '\r'
					       && **buf != '\n') {
						result += **buf;
						buf += 1;
					}
					if (buf.rest_length()) {
						if (**buf == '\n') {
							buf += 1;
							return ended = true;
						} else if (**buf == '\r') {
							buf += 1;
							if (buf.rest_length() && **buf == '\n') {
								buf += 1;
								return ended = true;
							}
							wait_LF = true;
							return false;
						} else {
							assert(false);
						}
					}
					return false;
				};
			}

			parser token(string& result)
			{
				bool ended = false;
				return P(buf, =, &result) {
					if (ended) return true;
					while (buf.rest_length()
					       && !isspace(**buf)) {
						result += **buf;
						buf += 1;
					}
					return ended = buf.rest_length();
				};
			}

			parser http_header(http_header_s& header)
			{
				function<string(string)> remove_colon = [](string s) {
					assert(!s.empty() && s.back() == ':');
					return s.substr(0, (int) s.length() - 1);
				};
				function<string(string)> trim = [](string s) {
					size_t skip = 0;
					while (skip < s.length()
					       && isspace(s[skip])) {
						++skip;
					}
					return s.substr(skip, (int) s.length() - skip);
				};
				auto p1 = fmap(remove_colon,
				               function<parser(string&)>(token))(header.key);
				auto p2 = fmap(trim,
				               function<parser(string&)>(until_endl))(header.value);
				return p1 >> p2;
			}

			parser http_headers(vector <http_header_s>& headers,
			                    size_t& content_length)
			{
				auto p = endline();
				bool in_header = false;
				bool ended = false;
				return P(buf, =, &headers, &content_length) {
					if (ended) return true;
					bool try_again = true;
					while (try_again) {
						try_again = false;
						if (!buf.rest_length()) return false;
						if (!in_header) {
							try {
								if (p(buf)) {
									return ended = true;
								}
							} catch (parse_error const&) {
								headers.emplace_back();
								p = http_header(headers.back());
								in_header = true;
								try_again = true;
							}
						} else {
							if (p(buf)) {
								auto& last = headers.back();
								if (strcasecmp(last.key.c_str(),
								               "Content-Length") == 0) {
									content_length = to_int<size_t>(last.value);
								} 
								in_header = false;
								p = endline();
								try_again = true;
							}
						} 
					}
					return false;
				};
			}

			parser http_body(char*& body, size_t& length)
			{
				bool start = true;
				size_t already_read = 0;
				return P(buf, =, &body, &length) {
					if (start) {
						body = new char[length];
						start = false;
					}
					size_t to_read = min(buf.rest_length(), length - already_read);
					for (size_t i = already_read;
					     i < already_read + to_read; i++) {
						body[i] = **buf;
						buf += 1;
					}
					already_read += to_read;
					return already_read == length;
				};
			}

			parser http_response(http_response_s& response)
			{
				function<string(string)> trim_back = [](string s) {
					int end = (int) s.length() - 1;
					while (end >= 0 && isspace(s[end])) --end;
					return s.substr(0, end + 1);
				};
				return token(response.version)
					>> character <' '>()
					>> integer(response.status)
					>> character <' '>()
					>> fmap(trim_back,
					        function<parser(string&)>(until_endl))(response.status_description)
					>> http_headers(response.headers, response.body_length)
					>> http_body(response.body, response.body_length);
			}

			parser http_request(http_request_s& request)
			{
				static size_t ignored_size;
				function<string(string)> trim_back = [](string s) {
					int end = (int) s.length() - 1;
					while (end >= 0 && isspace(s[end])) --end;
					return s.substr(0, end + 1);
				};
				return token(request.method)
					>> character <' '>()
					>> token(request.uri)
					>> character <' '>()
					>> fmap(trim_back,
					        function<parser(string&)>(until_endl))(request.version)
					>> http_headers(request.headers, ignored_size);
			}
		}
	}
}
