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
	          [&](response r)
	          {
		          cout << r.as_string();
		          main_loop::stop();
	          });
	
	main_loop::start();

	return 0;
}
