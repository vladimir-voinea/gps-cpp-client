#include "gpsd.hpp"

#include <gps.h>
#include <errno.h>

#include <spdlog/spdlog.h>
#include <cmath>

gpsd::gpsd()
{
    const auto gpsd_open_result = gps_open(GPSD_SHARED_MEMORY, NULL, &gps_data_);
    spdlog::info("gpsd_open result: {}", gpsd_open_result);

    if (gpsd_open_result != 0) {
        spdlog::error("code: {}, reason: {}", errno, gps_errstr(errno));
    }
}

gps_fix gpsd::run_until_fix()
{
    for(;;)
    {
        const auto gps_read_result = gps_read(&gps_data_, NULL, 0);
        if (gps_read_result == -1) 
        {
            spdlog::debug("error occured reading gps data. code: {}, reason: {}", errno, gps_errstr(errno));
        } 
        else 
        {
            if((gps_data_.status == STATUS_FIX) &&  (gps_data_.fix.mode == MODE_2D || gps_data_.fix.mode == MODE_3D) &&
                !std::isnan(gps_data_.fix.latitude) &&  !std::isnan(gps_data_.fix.longitude)) {
                    spdlog::info("Got fix. Latitude: {}, Longitude: {}", gps_data_.fix.latitude, gps_data_.fix.longitude);
                    gps_fix fix;
                    fix.latitude = gps_data_.fix.latitude;
                    fix.longitude = gps_data_.fix.longitude;

                    return fix;
            } else {
                spdlog::warn("Got GPS data but no fix containing lat/lon");
            }
        }
    }
}

gpsd::~gpsd()
{
    gps_close(&gps_data_);
}

void gpsd::set_observer(gpsd::observer_func func)
{
    observer_ = func;
}