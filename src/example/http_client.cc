#include <bits/stdc++.h>

#include "http/client.hh"
#include "http/main_loop.hh"

using namespace std;
using namespace http;

int main()
{
	client c("173.194.32.162");
	c.request(request::builder()
	          .set_method(request::GET)
	          .set_uri("/")
	          .create(),
	          [&](auto r)
	          {
		          r.raise();
		          cout << r.get().as_string();
		          main_loop::stop();
	          });
	
	try {
		main_loop::start();
	} catch (exception const& e) {
		cerr << e.what() << endl;
	}

	return 0;
}
