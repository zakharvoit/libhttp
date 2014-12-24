#ifndef MAIN_LOOP_HH
#define MAIN_LOOP_HH

#include <tcp/tcp.hh>

namespace http
{
	struct main_loop
	{
		static void start()
		{
			service.start();
		}

		static void stop()
		{
			service.stop();
		}

		static tcp::async::io_service& get_service()
		{
			return service;
		}

	private:
		static tcp::async::io_service service;
	};
}

#endif
