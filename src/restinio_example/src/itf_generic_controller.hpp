#pragma once

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;
using req_status_t = restinio::request_handling_status_t;

class itf_generic_controller
{
public:
    virtual ~itf_generic_controller() = default;

	virtual req_status_t on_list(const restinio::request_handle_t& req, rr::route_params_t ) const = 0;
	virtual req_status_t on_get_by(const restinio::request_handle_t& req, rr::route_params_t params ) = 0;
	virtual req_status_t on_new(const restinio::request_handle_t& req, rr::route_params_t ) = 0;
	virtual req_status_t on_update(const restinio::request_handle_t& req, rr::route_params_t params ) = 0;
	virtual req_status_t on_delete(const restinio::request_handle_t& req, rr::route_params_t params ) = 0;
};

// template<typename object_t>
// auto server_handler(std::vector<object_t>& object_collection)
// {
// 	auto router = std::make_unique<router_t>();
// 	auto handler = std::make_shared<generic_controller_t<object_t>>(std::ref(object_collection));

// 	auto by = [&]( auto method ) {
// 		using namespace std::placeholders;
// 		return std::bind(method, handler, _1, _2);
// 	};

// 	auto method_not_allowed = [](const auto & req, auto) {
// 			return req->create_response( restinio::status_method_not_allowed())
// 					.connection_close()
// 					.done();
// 		};

// 	// Handlers for '/' path.
// 	router->http_get("/", by(&generic_controller_t<object_t>::on_list));
// 	router->http_post("/", by(&generic_controller_t<object_t>::on_new));

// 	// Disable all other methods for '/'.
// 	router->add_handler(
// 			restinio::router::none_of_methods(
// 					restinio::http_method_get(), restinio::http_method_post()),
// 			"/", method_not_allowed);

// 	// Handlers for '/:id' path.
// 	router->http_get(
// 			R"(/:id(\d+))",
// 			by( &generic_controller_t<object_t>::on_get_by));
// 	router->http_put(
// 			R"(/:id(\d+))",
// 			by( &generic_controller_t<object_t>::on_update));
// 	router->http_delete(
// 			R"(/:id(\d+))",
// 			by( &generic_controller_t<object_t>::on_delete));

// 	// Disable all other methods for '/:id'.
// 	router->add_handler(
// 			restinio::router::none_of_methods(
// 					restinio::http_method_get(),
// 					restinio::http_method_post(),
// 					restinio::http_method_delete()),
// 			R"(/:id(\d+))", method_not_allowed);

// 	return router;
// }
