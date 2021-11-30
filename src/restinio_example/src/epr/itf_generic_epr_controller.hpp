#pragma once

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <restinio/router/easy_parser_router.hpp>
#include <json_dto/pub.hpp>

namespace rr = restinio::router;
using router_t = rr::easy_parser_router_t;
using req_status_t = restinio::request_handling_status_t;

class itf_generic_epr_controller
{
public:
    virtual ~itf_generic_epr_controller() = default;

	virtual req_status_t on_list(const restinio::request_handle_t& req) const = 0;
	virtual req_status_t on_get_by(const restinio::request_handle_t& req, std::uint64_t id) = 0;
	virtual req_status_t on_new(const restinio::request_handle_t& req) = 0;
	virtual req_status_t on_update(const restinio::request_handle_t& req, std::uint64_t id) = 0;
	virtual req_status_t on_delete(const restinio::request_handle_t& req, std::uint64_t id) = 0;
};
