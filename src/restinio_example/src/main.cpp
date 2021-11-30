#include "epr/generic_epr_controller.hpp"
#include "book.hpp"
#include "vector_container.hpp"
#include "spdlog_logger.hpp"
#include "per_request_data.hpp"

struct per_request_data
{
    std::string some_stuff;
};

struct traits : public restinio::default_traits_t
{
    using extra_data_factory_t = generic_per_request_data_factory<per_request_data>;
    using request_handler_t = restinio::router::generic_easy_parser_router_t<traits::extra_data_factory_t>;
    using logger_t = single_threaded_spdlog_logger_t;
};

int main()
{
	using namespace std::chrono;
    spdlog::info("Hello");

	try
	{
		vector_container<book_t> book_collection{
			{ "Agatha Christie", "Murder on the Orient Express" },
			{ "Agatha Christie", "Sleeping Murder" },
			{ "B. Stroustrup", "The C++ Programming Language" }
		};

        auto controller = generic_epr_controller_t<vector_container<book_t>, traits::extra_data_factory_t>(std::ref(book_collection));
		
        restinio::run(
			restinio::on_this_thread<traits>()
				.address( "0.0.0.0" )
				.request_handler(make_handler(controller))
				.read_next_http_message_timelimit( 10s )
				.write_http_response_timelimit( 1s )
                .extra_data_factory(std::make_shared<generic_per_request_data_factory<per_request_data>>())
				.handle_request_timeout( 1s ) );
	}
	catch( const std::exception & ex )
	{
		spdlog::error(ex.what());
		return 1;
	}

	return 0;
}