#include "http/response.hh"

using namespace std;
using namespace http;
using tcp::util::buffer;

const string response::DEFAULT_VERSION = "HTTP/1.1";

response::builder& response::builder::append(buffer const& b)
{
	buffer b_copy(b);
	is_finished = parse(b_copy);
	return *this;
}

response::builder& response::builder::set_version(
	string const& version)
{
	repr.version = version;

	return *this;
}

static string default_comment(enum response::status s)
{
	switch (s) {
	case response::OK:
		return "OK";
	case response::NOT_FOUND:
		return "Not found";
	}
}

response::builder& response::builder::set_status(enum status s)
{
	repr.status = s;
	if (repr.status_description == "") {
		repr.status_description = default_comment(s);
	}
	return *this;
}

response::builder& response::builder::set_comment(
	string const& comment)
{
	repr.status_description = comment;
	return *this;
}

response::builder& response::builder::add_header(
	string const& key, string const& value)
{
	repr.headers.push_back({key, value});
	return *this;
}

response::builder& response::builder::set_text(vector <char> const& chars)
{
	repr.body = chars;
	return *this;
}

response::builder& response::builder::set_text(string const& s)
{
	repr.body = vector <char>(s.begin(), s.end());
	return *this;
}

response::builder& response::builder::append_text(string const& s)
{
	for (char c : s) {
		repr.body.push_back(c);
	}

	return *this;
}

response response::builder::create()
{
	response result;
	result.status = static_cast <enum status>(repr.status);
	result.comment = repr.status_description;
	result.body = repr.body;
	result.headers.resize(repr.headers.size());
	for (size_t i = 0; i < result.headers.size(); i++) {
		result.headers[i] = make_pair(repr.headers[i].key,
		                              repr.headers[i].value);
	}

	return result;
}

buffer response::to_buffer() const
{
	string result;
	result += DEFAULT_VERSION + " ";
	result += to_string(status) + " ";
	result += comment + "\n";
	bool has_length = false;
	bool has_connection = false;
	for (auto const& h : headers) {
		result += h.first + ": " + h.second + "\n";
		if (h.first == "Content-Length") {
			has_length = true;
		}
		if (h.first == "Connection") {
			has_connection = true;
		}
	}
	if (!has_length) {
		result += "Content-Length: " + to_string(body.size()) + "\n";
	}
	if (!has_connection) {
		result += "Connection: close\n";
	}
	result += "\n";
	for (char c : body) {
		result += c;
	}
	return result;
}
