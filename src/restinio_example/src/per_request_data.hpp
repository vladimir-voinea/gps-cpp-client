#pragma once

#include <restinio/all.hpp>

template<typename T>
struct generic_per_request_data
{
    T data;
};

template<typename T>
class generic_per_request_data_factory
{
public:
    using data_t = generic_per_request_data<T>;
    
    void make_within(restinio::extra_data_buffer_t<data_t> buf)
    {
        new(buf.get()) data_t{};
    }
};
