#pragma once

#include <functional>
#include <vector>
#include <gps.h>

struct gps_fix
{
    double latitude;
    double longitude;
    double timestamp;
};

class gpsd
{
public:
    using observer_func = std::function<void(gps_fix)>;
public:
    gpsd();
    ~gpsd();

    void set_observer(observer_func func);
    gps_fix run_until_fix();

private:
    observer_func observer_;
    struct gps_data_t gps_data_;
};