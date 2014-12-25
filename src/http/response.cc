#include "http/response.hh"

using namespace std;
using namespace http;
using tcp::util::buffer;

response::builder& response::builder::append(buffer const& b)
{
	buffer b_copy(b);
	is_finished = parse(b_copy);
	return *this;
}

response response::builder::create()
{
	response result;
	result.status = static_cast <enum status>(repr.status);
	result.body = std::vector <char>(repr.body,
	                                 repr.body + repr.body_length);
	delete repr.body;
	result.headers.resize(repr.headers.size());
	for (size_t i = 0; i < result.headers.size(); i++) {
		result.headers[i] = make_pair(repr.headers[i].key,
		                              repr.headers[i].value);
	}

	return result;
}

// TODO: Rewrite this method completely
buffer response::to_buffer() const
{
	string result;
	result += "HTTP/1.1 ";
	result += to_string(status) + " ";
	result += "No comment\n";
	result += "Content-Length: 0\n\n";

	return result;
}
