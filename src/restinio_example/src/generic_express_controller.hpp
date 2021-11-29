#pragma once

#include "itf_generic_express_controller.hpp"

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>


template<typename object_t>
using generic_collection = std::vector< object_t >;

template<typename object_t>
class generic_express_controller_t : public itf_generic_express_controller
{
public :
	explicit generic_express_controller_t(std::vector<object_t> & objects)
		:	m_objects( objects )
	{}

	~generic_express_controller_t() override = default;

	generic_express_controller_t( const generic_express_controller_t & ) = delete;
	generic_express_controller_t( generic_express_controller_t && ) = delete;

	req_status_t on_list(const restinio::request_handle_t& req, rr::route_params_t) const override
	{
		auto resp = init_resp( req->create_response() );
		resp.set_body(json_dto::to_json(m_objects));

		return resp.done();
	}

	req_status_t on_get_by(const restinio::request_handle_t& req, rr::route_params_t params) override
	{
		const auto id = restinio::cast_to< std::uint32_t >( params[ "id" ] );

		auto resp = init_resp( req->create_response() );

		if(0 != id && id <= m_objects.size())
		{
			const auto & b = m_objects[id - 1];
			resp.set_body(json_dto::to_json<object_t>(b));
		}
		else
		{
			set_status(resp, restinio::status_not_found());
		}

		return resp.done();
	}

	req_status_t on_new(const restinio::request_handle_t& req, rr::route_params_t ) override
	{
		auto resp = init_resp( req->create_response() );

		try
		{
			m_objects.emplace_back(json_dto::from_json<object_t>(req->body()));
		}
		catch(const std::exception& ex)
		{
			spdlog::error(ex.what());
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	req_status_t on_update(const restinio::request_handle_t& req, rr::route_params_t params) override
	{
		const auto id = restinio::cast_to<std::uint32_t>(params[ "id" ]);

		auto resp = init_resp(req->create_response());

		try
		{
			auto b = json_dto::from_json<object_t>(req->body());

			if(0 != id && id <= m_objects.size())
			{
				m_objects[id - 1] = b;
			}
			else
			{
				set_status(resp, restinio::status_not_found());
			}
		}
		catch(const std::exception& ex)
		{
			spdlog::error(ex.what());
			mark_as_bad_request(resp);
		}

		return resp.done();
	}

	req_status_t on_delete(const restinio::request_handle_t& req, rr::route_params_t params ) override
	{
		const auto id = restinio::cast_to< std::uint32_t >( params[ "id" ] );

		auto resp = init_resp(req->create_response());

		if(0 != id && id <= m_objects.size())
		{
			const auto& b = m_objects[id - 1];
			resp.set_body(json_dto::to_json<object_t>(b));

			m_objects.erase(m_objects.begin() + (id - 1));
		}
		else
		{
			set_status(resp, restinio::status_not_found());
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
			.append_header("Content-Type", "application/json; charset=utf-8");

		return resp;
	}

	template <typename RESP>
	static void mark_as_bad_request(RESP& resp)
	{
		resp.header().status_line(restinio::status_bad_request());
	}

	template <typename RESP, typename STATUS>
	static void set_status(RESP& resp, const STATUS& status)
	{
		resp.header().status_line(status);
	}
};

template<typename controller_t>
auto make_handler(controller_t& controller)
{
	auto router = std::make_unique<router_t>();

	auto by = [&](auto method) {
		using namespace std::placeholders;
		return std::bind(method, std::addressof(controller), _1, _2);
	};

	auto method_not_allowed = [](const auto & req, auto) {
			return req->create_response(restinio::status_method_not_allowed())
					.connection_close()
					.done();
		};

	// Handlers for '/' path.
	router->http_get("/", by(&controller_t::on_list));
	router->http_post("/", by(&controller_t::on_new));

	// Disable all other methods for '/'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(), restinio::http_method_post()),
			"/", method_not_allowed);

	// Handlers for '/:id' path.
	router->http_get(R"(/:id(\d+))", by(&controller_t::on_get_by));
	router->http_put(R"(/:id(\d+))", by(&controller_t::on_update));
	router->http_delete(R"(/:id(\d+))", by(&controller_t::on_delete));

	// Disable all other methods for '/:id'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(),
					restinio::http_method_post(),
					restinio::http_method_delete()),
			R"(/:id(\d+))", method_not_allowed);

	return router;
}
