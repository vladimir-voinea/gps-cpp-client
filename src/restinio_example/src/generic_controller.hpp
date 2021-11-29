#pragma once

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

template<typename object_t>
using generic_collection = std::vector< object_t >;

template<typename object_t>
class generic_controller_t
{
public :
	explicit generic_controller_t( std::vector<object_t> & objects )
		:	m_objects( objects )
	{}

	generic_controller_t( const generic_controller_t & ) = delete;
	generic_controller_t( generic_controller_t && ) = delete;

	auto on_list(const restinio::request_handle_t& req, rr::route_params_t ) const
	{
		auto resp = init_resp( req->create_response() );

		resp.set_body(
			"Object collection (object count: " +
				std::to_string( m_objects.size() ) + ")\n" );

		for( std::size_t i = 0; i < m_objects.size(); ++i )
		{
			resp.append_body( std::to_string( i + 1 ) + ". " );
			const auto & b = m_objects[ i ];
			resp.append_body(json_dto::to_json<object_t>(b) + '\n');
		}

		return resp.done();
	}

	auto on_get_by(const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto id = restinio::cast_to< std::uint32_t >( params[ "id" ] );

		auto resp = init_resp( req->create_response() );

		if( 0 != id && id <= m_objects.size() )
		{
			const auto & b = m_objects[ id - 1 ];
			resp.set_body(json_dto::to_json<object_t>(b) + '\n');
		}
		else
		{
			resp.set_body(
				"No object with #" + std::to_string( id ) + "\n" );
		}

		return resp.done();
	}

	auto on_new(const restinio::request_handle_t& req, rr::route_params_t )
	{
		auto resp = init_resp( req->create_response() );

		try
		{
			m_objects.emplace_back(
				json_dto::from_json<object_t>( req->body() ) );
		}
		catch( const std::exception & /*ex*/ )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_update(const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto id = restinio::cast_to< std::uint32_t >( params[ "id" ] );

		auto resp = init_resp( req->create_response() );

		try
		{
			auto b = json_dto::from_json< object_t >( req->body() );

			if( 0 != id && id <= m_objects.size() )
			{
				m_objects[ id - 1 ] = b;
			}
			else
			{
				mark_as_bad_request( resp );
				resp.set_body( "No object with #" + std::to_string( id ) + "\n" );
			}
		}
		catch( const std::exception & /*ex*/ )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_delete(const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto id = restinio::cast_to< std::uint32_t >( params[ "id" ] );

		auto resp = init_resp( req->create_response() );

		if( 0 != id && id <= m_objects.size() )
		{
			const auto & b = m_objects[ id - 1 ];
			resp.set_body(json_dto::to_json<object_t>(b) + '\n');

			m_objects.erase( m_objects.begin() + ( id - 1 ) );
		}
		else
		{
			resp.set_body(
				"No object with #" + std::to_string( id ) + "\n" );
		}

		return resp.done();
	}

private :
	std::vector<object_t> & m_objects;

	template <typename RESP>
	static RESP
	init_resp(RESP resp)
	{
		resp
			.append_header("Server", "RESTinio sample server /v.0.6")
			.append_header_date_field()
			.append_header("Content-Type", "text/plain; charset=utf-8");

		return resp;
	}

	template <typename RESP>
	static void
	mark_as_bad_request(RESP& resp)
	{
		resp.header().status_line(restinio::status_bad_request());
	}
};

template<typename object_t>
auto server_handler(std::vector<object_t>& object_collection)
{
	auto router = std::make_unique<router_t>();
	auto handler = std::make_shared<generic_controller_t<object_t>>(std::ref(object_collection));

	auto by = [&]( auto method ) {
		using namespace std::placeholders;
		return std::bind(method, handler, _1, _2);
	};

	auto method_not_allowed = [](const auto & req, auto) {
			return req->create_response( restinio::status_method_not_allowed())
					.connection_close()
					.done();
		};

	// Handlers for '/' path.
	router->http_get("/", by(&generic_controller_t<object_t>::on_list));
	router->http_post("/", by(&generic_controller_t<object_t>::on_new));

	// Disable all other methods for '/'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(), restinio::http_method_post()),
			"/", method_not_allowed);

	// Handlers for '/:id' path.
	router->http_get(
			R"(/:id(\d+))",
			by( &generic_controller_t<object_t>::on_get_by));
	router->http_put(
			R"(/:id(\d+))",
			by( &generic_controller_t<object_t>::on_update));
	router->http_delete(
			R"(/:id(\d+))",
			by( &generic_controller_t<object_t>::on_delete));

	// Disable all other methods for '/:id'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(),
					restinio::http_method_post(),
					restinio::http_method_delete()),
			R"(/:id(\d+))", method_not_allowed);

	return router;
}
