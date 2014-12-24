#include "http/request.hh"

#include <tcp/tcp.hh>

using namespace tcp;
using namespace http;
using namespace std;

request::builder& request::builder::append(util::buffer const& buf)
{
	util::buffer b_copy(buf);
	is_finished = parse(b_copy);
	return *this;
	
}

request request::builder::create()
{
	request result;
	result.method = GET; // TODO: Parse real method, it's just stub.
	result.uri.uri_path = repr.uri;
	// TODO: Bind other fields
	return result;
}

// TODO: Rewrite this method.
util::buffer request::to_buffer() const
{
	string result;
	result += "GET ";
	result += uri.uri_path + " ";
	result += "HTTP/1.1\n";
	result += "Host: google.com\n";
	result += "\n\n";

	return result;
}
