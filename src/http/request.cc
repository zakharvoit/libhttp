#include "http/request.hh"

#include <tcp/tcp.hh>

using namespace tcp;
using namespace http;
using namespace std;

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
