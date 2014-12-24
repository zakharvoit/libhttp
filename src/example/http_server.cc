#include <bits/stdc++.h>

#include "http/server.hh"
#include "http/main_loop.hh"

using namespace std;
using namespace http;

int main()
{
	server s("127.0.0.1", [&](auto r)
	         {
		         auto b = r.to_buffer();
		         while (b.rest_length()) {
			         cout << **b;
			         b += 1;
		         }
	         }, 33334);

	main_loop::start();

	return 0;
}
