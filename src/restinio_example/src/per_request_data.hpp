#pragma once

#include <restinio/all.hpp>


template<typename T>
class generic_per_request_data_factory
{
public:
    struct data_t
    {
        T data;
    };

    void make_within(restinio::extra_data_buffer_t<data_t> buf)
    {
        new(buf.get()) data_t{};
    }
};
