#pragma once

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

struct example_t
{
	example_t() = default;

	example_t(
		std::string attribute1,
		std::string attribute2 )
		:	m_attribute1{ std::move( attribute1 ) }
		,	m_attribute2{ std::move( attribute2 ) }
	{}

	template < typename JSON_IO >
	void
	json_io( JSON_IO & io )
	{
		io
			& json_dto::mandatory( "attribute1", m_attribute1 )
			& json_dto::mandatory( "attribute2", m_attribute2 );
	}

	std::string m_attribute1;
	std::string m_attribute2;
};

using example_collection_t = std::vector< example_t >;

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;

class examples_handler_t
{
public :
	explicit examples_handler_t( example_collection_t & examples )
		:	m_examples( examples )
	{}

	examples_handler_t( const examples_handler_t & ) = delete;
	examples_handler_t( examples_handler_t && ) = delete;

	auto on_examples_list(
		const restinio::request_handle_t& req, rr::route_params_t ) const
	{
		auto resp = init_resp( req->create_response() );

		resp.set_body(
			"Example collection (example count: " +
				std::to_string( m_examples.size() ) + ")\n" );

		for( std::size_t i = 0; i < m_examples.size(); ++i )
		{
			resp.append_body( std::to_string( i + 1 ) + ". " );
			const auto & b = m_examples[ i ];
			resp.append_body( b.m_attribute2 + "[" + b.m_attribute1 + "]\n" );
		}

		return resp.done();
	}

	auto on_example_get(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto examplenum = restinio::cast_to< std::uint32_t >( params[ "examplenum" ] );

		auto resp = init_resp( req->create_response() );

		if( 0 != examplenum && examplenum <= m_examples.size() )
		{
			const auto & b = m_examples[ examplenum - 1 ];
			resp.set_body(
				"Example #" + std::to_string( examplenum ) + " is: " +
					b.m_attribute2 + " [" + b.m_attribute1 + "]\n" );
		}
		else
		{
			resp.set_body(
				"No example with #" + std::to_string( examplenum ) + "\n" );
		}

		return resp.done();
	}

	auto on_attribute1_get(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		auto resp = init_resp( req->create_response() );
		try
		{
			auto attribute1 = restinio::utils::unescape_percent_encoding( params[ "attribute1" ] );

			resp.set_body( "Examples of " + attribute1 + ":\n" );

			for( std::size_t i = 0; i < m_examples.size(); ++i )
			{
				const auto & b = m_examples[ i ];
				if( attribute1 == b.m_attribute1 )
				{
					resp.append_body( std::to_string( i + 1 ) + ". " );
					resp.append_body( b.m_attribute2 + "[" + b.m_attribute1 + "]\n" );
				}
			}
		}
		catch( const std::exception & )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_new_example(
		const restinio::request_handle_t& req, rr::route_params_t )
	{
		auto resp = init_resp( req->create_response() );

		try
		{
			m_examples.emplace_back(
				json_dto::from_json< example_t >( req->body() ) );
		}
		catch( const std::exception & /*ex*/ )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_example_update(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto examplenum = restinio::cast_to< std::uint32_t >( params[ "examplenum" ] );

		auto resp = init_resp( req->create_response() );

		try
		{
			auto b = json_dto::from_json< example_t >( req->body() );

			if( 0 != examplenum && examplenum <= m_examples.size() )
			{
				m_examples[ examplenum - 1 ] = b;
			}
			else
			{
				mark_as_bad_request( resp );
				resp.set_body( "No example with #" + std::to_string( examplenum ) + "\n" );
			}
		}
		catch( const std::exception & /*ex*/ )
		{
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	auto on_example_delete(
		const restinio::request_handle_t& req, rr::route_params_t params )
	{
		const auto examplenum = restinio::cast_to< std::uint32_t >( params[ "examplenum" ] );

		auto resp = init_resp( req->create_response() );

		if( 0 != examplenum && examplenum <= m_examples.size() )
		{
			const auto & b = m_examples[ examplenum - 1 ];
			resp.set_body(
				"Delete example #" + std::to_string( examplenum ) + ": " +
					b.m_attribute2 + "[" + b.m_attribute1 + "]\n" );

			m_examples.erase( m_examples.begin() + ( examplenum - 1 ) );
		}
		else
		{
			resp.set_body(
				"No example with #" + std::to_string( examplenum ) + "\n" );
		}

		return resp.done();
	}

private :
	example_collection_t & m_examples;

	template < typename RESP >
	static RESP
	init_resp( RESP resp )
	{
		resp
			.append_header( "Server", "RESTinio sample server /v.0.6" )
			.append_header_date_field()
			.append_header( "Content-Type", "text/plain; charset=utf-8" );

		return resp;
	}

	template < typename RESP >
	static void
	mark_as_bad_request( RESP & resp )
	{
		resp.header().status_line( restinio::status_bad_request() );
	}
};

auto server_handler( example_collection_t & example_collection )
{
	auto router = std::make_unique< router_t >();
	auto handler = std::make_shared< examples_handler_t >( std::ref(example_collection) );

	auto by = [&]( auto method ) {
		using namespace std::placeholders;
		return std::bind( method, handler, _1, _2 );
	};

	auto method_not_allowed = []( const auto & req, auto ) {
			return req->create_response( restinio::status_method_not_allowed() )
					.connection_close()
					.done();
		};

	// Handlers for '/' path.
	router->http_get( "/", by( &examples_handler_t::on_examples_list ) );
	router->http_post( "/", by( &examples_handler_t::on_new_example ) );

	// Disable all other methods for '/'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(), restinio::http_method_post() ),
			"/", method_not_allowed );

	// Handler for '/attribute1/:attribute1' path.
	router->http_get( "/attribute1/:attribute1", by( &examples_handler_t::on_attribute1_get ) );

	// Disable all other methods for '/attribute1/:attribute1'.
	router->add_handler(
			restinio::router::none_of_methods( restinio::http_method_get() ),
			"/attribute1/:attribute1", method_not_allowed );

	// Handlers for '/:examplenum' path.
	router->http_get(
			R"(/:examplenum(\d+))",
			by( &examples_handler_t::on_example_get ) );
	router->http_put(
			R"(/:examplenum(\d+))",
			by( &examples_handler_t::on_example_update ) );
	router->http_delete(
			R"(/:examplenum(\d+))",
			by( &examples_handler_t::on_example_delete ) );

	// Disable all other methods for '/:examplenum'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(),
					restinio::http_method_post(),
					restinio::http_method_delete() ),
			R"(/:examplenum(\d+))", method_not_allowed );

	return router;
}
