#include "gpsd.hpp"

#include <gps.h>
#include <errno.h>

#include <spdlog/spdlog.h>
#include <cmath>
#include <sys/time.h>

gpsd::gpsd()
    : gps_data_(std::make_unique<gps_data_t>())
{
    const auto gpsd_host_env = ::getenv("GPSD_HOST");
    const auto gpsd_port_env = ::getenv("GPSD_PORT");
    spdlog::info("GPSD host/port: {}:{}", gpsd_host_env, gpsd_port_env);

    const auto gpsd_open_result = gps_open(gpsd_host_env, gpsd_port_env, gps_data_.get());
    spdlog::info("gpsd_open result: {}", gpsd_open_result);

    if (gpsd_open_result != 0) 
    {
        spdlog::error("code: {}, reason: {}", errno, gps_errstr(errno));
        gps_data_.reset();
    }
}

bool gpsd::good() const
{
    return gps_data_.get() != nullptr;
}

gps_fix gpsd::run_until_fix()
{
    if(!gps_data_)
    {
        throw std::runtime_error{"GPSD connection wasn't made"};
    }

    for(;;)
    {
        const auto gps_read_result = gps_read(gps_data_.get(), NULL, 0);
        if (gps_read_result == -1) 
        {
            spdlog::debug("error occured reading gps data. code: {}, reason: {}", errno, gps_errstr(errno));
        } 
        else 
        {
            if(gps_data_->status == STATUS_FIX && gps_data_->fix.mode == MODE_2D || gps_data_->fix.mode == MODE_3D &&
                !std::isnan(gps_data_->fix.latitude) &&  !std::isnan(gps_data_->fix.longitude)) 
            {
                gps_fix fix;
                fix.latitude = gps_data_->fix.latitude;
                fix.longitude = gps_data_->fix.longitude;

                if(!std::isnan(gps_data_->fix.altHAE)) {
                    fix.altitude = gps_data_->fix.altHAE;
                }

                if(!std::isnan(gps_data_->fix.speed)) {
                    fix.speed = gps_data_->fix.speed;
                }

                if(!std::isnan(gps_data_->fix.track)) {
                    fix.bearing = gps_data_->fix.track;
                }

                if(!std::isnan(gps_data_->fix.climb)) {
                    fix.climb = gps_data_->fix.climb;
                }

                // timeval tv;
                // TIMESPEC_TO_TIMEVAL(&gps_data_->fix.time, &tv);
                // fix.timestamp = (tv.tv_sec * 1000000ULL + tv.tv_usec) * 1000;

                return fix;
            }
        }
    }
}

gpsd::~gpsd()
{
    gps_close(gps_data_.get());
}

void gpsd::set_observer(gpsd::observer_func func)
{
    observer_ = func;
}