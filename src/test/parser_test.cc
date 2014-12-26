#include <gtest/gtest.h>

#include "http/impl/parser.hh"

#include <string>
#include <vector>

using namespace std;
using namespace http::impl::parser;
using namespace tcp::util;

TEST(parser, primitives)
{
	string tok = "token";
	string toendline = "Toendline";
	int value = 100;

	string text = tok + " "
		+ toendline + "\r\n"
		+ toendline + "\n"
		+ to_string(value) + " ";

	string tok_res;
	string toendline_res;
	string toendline_res2;
	int value_res;

	buffer buf(text);

	ASSERT_TRUE(token(tok_res)(buf));
	ASSERT_TRUE(character<' '>()(buf));
	ASSERT_TRUE(until_endl(toendline_res)(buf));
	ASSERT_TRUE(until_endl(toendline_res2)(buf));
	ASSERT_TRUE(integer(value_res)(buf));

	ASSERT_EQ(tok, tok_res);
	ASSERT_EQ(toendline, toendline_res);
	ASSERT_EQ(toendline, toendline_res2);
	ASSERT_EQ(value, value_res);
}

TEST(parser, header)
{
	string key = "key";
	string value = "value";
	buffer header = key + ": " + value + "\r\n";
	http_header_s result;
	ASSERT_TRUE(http_header(result)(header));
	ASSERT_EQ(key, result.key);
	ASSERT_EQ(value, result.value);
}

TEST(parser, headers)
{
	string key = "key";
	string value = "value";
	buffer headers = key + ": " + value + "\r\n"
		"Content-Length: 10\r\n\r\n";
	vector <http_header_s> result;
	size_t length;
	ASSERT_TRUE(http_headers(result, length)(headers));
	ASSERT_EQ(10, length);
	ASSERT_EQ(key, result[0].key);
	ASSERT_EQ(value, result[0].value);
	ASSERT_EQ("Content-Length", result[1].key);
	ASSERT_EQ("10", result[1].value);
}

TEST(parser, http_response)
{
	string hw = "Hello World!\n";
	buffer response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\n"s
		+ "Hello World!\n"s;
	http_response_s result;
	http_response_s expected = {
		.version = "HTTP/1.1",
		.status = 200,
		.status_description = "OK",
		.headers = {
			http_header_s{
				"Content-Length", "13"
			}
		},
		.body = vector <char>(hw.begin(), hw.end()),
		.body_length = 13
	};
	auto p = http_response(result);
	ASSERT_TRUE(p(response));
	ASSERT_EQ(expected, result);
}

TEST(parser, http_request)
{
	buffer request = "GET /index.html HTTP/1.1\r\n"s
		+ "Host: example.org\r\n"
		+ "Content-Length: 5\r\n"
		+ "\r\n"
		+ "Hello";
	http_request_s result;
	http_request_s expected = {
		.method = "GET",
		.uri = "/index.html",
		.version = "HTTP/1.1",
		.headers = {
			{
				"Host", "example.org"
			},
			{
				"Content-Length", "5"
			}
		},
		.body = {'H', 'e', 'l', 'l', 'o'}
	};
	auto p = http_request(result);
	ASSERT_TRUE(p(request));
	ASSERT_EQ(expected, result);
}
