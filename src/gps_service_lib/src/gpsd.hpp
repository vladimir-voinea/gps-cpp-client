#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <optional>

struct gps_fix
{
    double timestamp;

    double latitude;
    double longitude;
    std::optional<double> altitude;

    std::optional<double> bearing;
    std::optional<double> speed;
    std::optional<double> climb;
};

struct gps_data_t;

class gpsd
{
public:
    using observer_func = std::function<void(gps_fix)>;
public:
    gpsd();
    ~gpsd();

    bool good() const;

    void set_observer(observer_func func);
    gps_fix run_until_fix();

private:
    observer_func observer_;
    std::unique_ptr<gps_data_t> gps_data_;
};