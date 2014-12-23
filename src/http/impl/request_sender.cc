#include "http/impl/request_sender.hh"

#include "http/main_loop.hh"

using namespace tcp::async;
using namespace tcp::util;
using namespace http::impl;

using http::main_loop;

request_sender::request_sender(http::request const& request,
			       address const& addr,
			       http::on_response_cb const& on_response)
    : client(),
      on_response(on_response),
      builder()
{
    client.connect(main_loop::get_service(),
		   addr,
		   [=](maybe <nothing>&& e)
		   {
		       e.raise(); // TODO: Handle errors better

		       this->on_connect(request);
		   }
	);
}

void request_sender::on_connect(http::request const& request)
{
    client.write(main_loop::get_service(),
		 request.to_buffer(),
		 [=](maybe <nothing>&& e)
		 {
		     e.raise(); // TODO: Handler errors better
		     this->on_wrote();
		 }
	)
	
}

void request_sender::on_wrote()
{
    client.read_some(main_loop::get_service(), [=](maybe<buffer>&& b)
		     {
			 this->on_read(std::move(b));
		     });
}

void request_sender::on_read(maybe<buffer>&& b)
{
    b.raise(); // TODO: Handle errors better.

    builder.append(b.get());
    if (builder.finished()) {
	on_response(builder.create());
    } else {
	on_wrote();
    }
}
