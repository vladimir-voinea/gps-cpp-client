#pragma once

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <json_dto/pub.hpp>

namespace rr = restinio::router;
using router_t = rr::express_router_t<>;
using req_status_t = restinio::request_handling_status_t;

class itf_generic_express_controller
{
public:
    virtual ~itf_generic_express_controller() = default;

	virtual req_status_t on_list(const restinio::request_handle_t& req, rr::route_params_t ) const = 0;
	virtual req_status_t on_get_by(const restinio::request_handle_t& req, rr::route_params_t params ) = 0;
	virtual req_status_t on_new(const restinio::request_handle_t& req, rr::route_params_t ) = 0;
	virtual req_status_t on_update(const restinio::request_handle_t& req, rr::route_params_t params ) = 0;
	virtual req_status_t on_delete(const restinio::request_handle_t& req, rr::route_params_t params ) = 0;
};
