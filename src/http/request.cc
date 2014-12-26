#include "http/request.hh"
#include "http/response.hh"

#include <tcp/tcp.hh>

#include <iostream>

using namespace tcp;
using namespace http;
using namespace std;

request::builder& request::builder::append(util::buffer const& buf)
{
	util::buffer b_copy(buf);
	is_finished = parse(b_copy);
	return *this;
}

string to_string(enum request::method m)
{
	switch (m) {
	case request::GET: return "GET";
	case request::POST: return "POST";
	}
}

enum request::method from_string(string const& m)
{
	if (m == "GET") return request::GET;
	else return request::POST;
}

request::builder& request::builder::set_method(enum request::method m)
{
	repr.method = to_string(m);
	return *this;
}

request::builder& request::builder::set_uri(string const& uri)
{
	repr.uri = uri;
	return *this;
}

request::builder& request::builder::add_header(string const& key,
                                               string const& value)
{
	repr.headers.push_back({key, value});
	return *this;
}

request request::builder::create()
{
	request result;
	result.method = from_string(repr.method);
	result.uri = repr.uri;
	result.headers.resize(repr.headers.size());
	for (size_t i = 0; i < result.headers.size(); i++) {
		result.headers[i] = make_pair(repr.headers[i].key,
		                              repr.headers[i].value);
	}

	return result;
}

util::buffer request::to_buffer() const
{
	string result;
	result += to_string(method) + " ";
	result += uri + " ";
	result += response::DEFAULT_VERSION + "\n";

	bool has_connection = false;

	for (auto const& h : headers) {
		if (h.first == "Connection") {
			has_connection = true;
		}
		result += h.first + ": " + h.second + "\n";
	}
	if (!has_connection) {
		result += "Connection: close\n";
	}
	result += "\n";

	return result;
}
