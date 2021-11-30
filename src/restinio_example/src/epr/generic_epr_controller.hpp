#pragma once

#include "itf_generic_epr_controller.hpp"

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

namespace
{

template<typename R, typename T, typename U, typename... Args>
std::function<R(Args...)> Attach(R (T::*f)(Args...), U p)
{
    return [p,f](Args... args)->R { return (p->*f)(std::forward<Args>(args)...); };
};

template<typename R, typename T, typename U, typename... Args>
std::function<R(Args...)> Attach(R (T::*f)(Args...) const, U p)
{
    return [p,f](Args... args)->R { return (p->*f)(std::forward<Args>(args)...); };
};

// template<typename R, typename T, typename U, typename Args>
// std::function<R(const Args&)> Attach(R (T::*f)(const Args&), U p)
// {
//     return [p,f](const Args& args)->R { return (p->*f)(args); };
// };

// template<typename R, typename T, typename U, typename Arg>
// std::function<R(Arg&&)> Attach(R (T::*f)(Arg&&), U p)
// {
//     return [p,f](Arg&& arg)->R { return (p->*f)(arg); };
// };

}

template<typename collection_t>
class generic_epr_controller_t : public itf_generic_epr_controller
{
private:
	using object_t = typename collection_t::value_type;
public:
	explicit generic_epr_controller_t(collection_t& objects)
		:	m_collection( objects )
	{}

	~generic_epr_controller_t() override = default;

	generic_epr_controller_t( const generic_epr_controller_t & ) = delete;
	generic_epr_controller_t( generic_epr_controller_t && ) = delete;

	req_status_t on_list(const restinio::request_handle_t& req) const override
	{
		auto resp = init_resp( req->create_response() );
		resp.set_body(json_dto::to_json(m_collection));

		return resp.done();
	}

	req_status_t on_get_by(const restinio::request_handle_t& req, std::uint64_t id) override
	{
		auto resp = init_resp( req->create_response() );

		if(auto it = m_collection.find(id); it != m_collection.end())
		{
			const auto & b = *it;
			resp.set_body(json_dto::to_json<object_t>(b));
		}
		else
		{
			set_status(resp, restinio::status_not_found());
		}

		return resp.done();
	}

	req_status_t on_new(const restinio::request_handle_t& req) override
	{
		auto resp = init_resp( req->create_response() );

		try
		{
			m_collection.add(json_dto::from_json<object_t>(req->body()));
		}
		catch(const std::exception& ex)
		{
			spdlog::error(ex.what());
			mark_as_bad_request( resp );
		}

		return resp.done();
	}

	req_status_t on_update(const restinio::request_handle_t& req, std::uint64_t id) override
	{
		auto resp = init_resp(req->create_response());

		try
		{
			auto b = json_dto::from_json<object_t>(req->body());

			if(auto it = m_collection.find(id); it != m_collection.end())
			{
				*it = b;
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

	req_status_t on_delete(const restinio::request_handle_t& req, std::uint64_t id) override
	{
		auto resp = init_resp(req->create_response());

		if(auto it = m_collection.find(id); it != m_collection.end())
		{
			const auto& b = *it;
			resp.set_body(json_dto::to_json<object_t>(b));

			m_collection.erase(it);
		}
		else
		{
			set_status(resp, restinio::status_not_found());
		}

		return resp.done();
	}

private :
	collection_t& m_collection;

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
    namespace epr = restinio::router::easy_parser_router;
	auto router = std::make_unique<router_t>();

	auto by = [&](auto method) {
		using namespace std::placeholders;
		return Attach(method, std::addressof(controller));
	};

	auto method_not_allowed = [](const auto & req, auto&& ...args) {
			return req->create_response(restinio::status_method_not_allowed())
					.connection_close()
					.done();
		};

	// Handlers for '/' path.
	router->http_get(epr::path_to_params("/"), by(&controller_t::on_list));
	router->http_post(epr::path_to_params("/"), by(&controller_t::on_new));

	// Disable all other methods for '/'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(), restinio::http_method_post()),
			epr::path_to_params("/"), method_not_allowed);

    auto id_p = epr::non_negative_decimal_number_p<std::uint64_t>();

	// Handlers for '/:id' path.
	router->http_get(epr::path_to_params("/", id_p), by(&controller_t::on_get_by));
	router->http_put(epr::path_to_params("/", id_p), by(&controller_t::on_update));
	router->http_delete(epr::path_to_params("/", id_p), by(&controller_t::on_delete));

	// Disable all other methods for '/:id'.
	router->add_handler(
			restinio::router::none_of_methods(
					restinio::http_method_get(),
					restinio::http_method_post(),
					restinio::http_method_delete()),
			epr::path_to_params("/", id_p), method_not_allowed);

	return router;
}
