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
		               if (!e) {
			               this->on_connect(e.get_error());
		               } else {
			               this->on_connect(request);
		               }
	               }
		);
}

void request_sender::on_connect(tcp::util::maybe<http::request> const& request)
{
	if (!request) {
		on_response(request.get_error());
		return;
	}
	client.write(main_loop::get_service(),
	             request.get().to_buffer(),
	             [=](maybe <nothing>&& e)
	             {
		             if (!e) {
			             on_response(e.get_error());
			             return;
		             }
		             this->on_wrote();
	             }
		);
		
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
	if (!b) {
		on_response(b.get_error());
		return;
	}
	
	builder.append(b.get());
	if (builder.finished()) {
		on_response(builder.create());
	} else {
		on_wrote();
	}
}
