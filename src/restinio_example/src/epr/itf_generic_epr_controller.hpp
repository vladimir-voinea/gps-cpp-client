#pragma once

#include <spdlog/spdlog.h>
#include <restinio/all.hpp>
#include <restinio/router/easy_parser_router.hpp>
#include <json_dto/pub.hpp>

namespace rr = restinio::router;
using req_status_t = restinio::request_handling_status_t;

template<typename per_request_data_factory_t>
class itf_generic_epr_controller
{
public:
    using per_request_data_factory_type = per_request_data_factory_t;
    using request_handle_t = restinio::generic_request_handle_t<typename per_request_data_factory_t::data_t>;

public:
    virtual ~itf_generic_epr_controller() = default;

	virtual req_status_t on_list(const request_handle_t&) const = 0;
	virtual req_status_t on_get_by(const request_handle_t&, std::uint64_t id) = 0;
	virtual req_status_t on_new(const request_handle_t&) = 0;
	virtual req_status_t on_update(const request_handle_t&, std::uint64_t id) = 0;
	virtual req_status_t on_delete(const request_handle_t&, std::uint64_t id) = 0;
};
