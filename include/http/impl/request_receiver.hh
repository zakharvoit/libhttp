#ifndef HTTP_REQUEST_RECEIVER_HH
#define HTTP_REQUEST_RECEIVER_HH

#include "http/callbacks.hh"
#include "http/request.hh"

#include <tcp/tcp.hh>

namespace http
{
	struct request_receiver
	{
		request_receiver(int id);
		request_receiver(tcp::async::client&&,
		                 on_request_cb const&,
		                 std::function<void(int)> on_finish);

		bool operator<(request_receiver const& rhs) const
		{
			return id < rhs.id;
		}

	private:
		void on_read_some(tcp::util::maybe<tcp::util::buffer>&&);

		int id;
		tcp::async::client client;
		on_request_cb on_request;
		std::function<void(int)> on_finish;
		request::builder builder;
	};
}

#endif
