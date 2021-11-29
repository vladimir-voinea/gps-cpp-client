#include "express/generic_express_controller.hpp"
#include "book.hpp"
#include "vector_container.hpp"
#include "spdlog_logger.hpp"

int main()
{
	using namespace std::chrono;

	try
	{
		using traits_t =
			restinio::traits_t<
				restinio::asio_timer_manager_t,
				restinio::single_threaded_ostream_logger_t,
				router_t >;

		vector_container<book_t> book_collection{
			{ "Agatha Christie", "Murder on the Orient Express" },
			{ "Agatha Christie", "Sleeping Murder" },
			{ "B. Stroustrup", "The C++ Programming Language" }
		};
        auto controller = generic_express_controller_t<vector_container<book_t>>(std::ref(book_collection));
		
        restinio::run(
			restinio::on_this_thread<traits_t>()
				.address( "0.0.0.0" )
				.request_handler(make_handler(controller))
				.read_next_http_message_timelimit( 10s )
				.write_http_response_timelimit( 1s )
				.handle_request_timeout( 1s ) );
	}
	catch( const std::exception & ex )
	{
		spdlog::error(ex.what());
		return 1;
	}

	return 0;
}