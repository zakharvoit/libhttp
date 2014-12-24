#include <gtest/gtest.h>

#include "http/client.hh"
#include "http/main_loop.hh"

TEST(client, check_google)
{
	using namespace http;

	ASSERT_NO_THROW(
		client c("173.194.32.166");
		c.request({request::GET, {"/"}}, [](auto r) {
				auto m = r.get_body();
				main_loop::stop();
			});
		http::main_loop::start();
		);
}
