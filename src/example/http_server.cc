#include <bits/stdc++.h>

#include "http/server.hh"
#include "http/main_loop.hh"

using namespace std;
using namespace http;

int main()
{
	try {
		size_t count = 0;
		server s("127.0.0.1", [&](auto r, auto peer)
		         {
			         peer.send(response::builder()
			                   .set_text("You are " + to_string(++count) + " visitor.")
			                   .create());
		         }, 33334);
		
		main_loop::start();
	} catch (exception const& e) {
		cerr << e.what() << endl;
	}

	return 0;
}
