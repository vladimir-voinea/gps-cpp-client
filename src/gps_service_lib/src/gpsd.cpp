#include "gpsd.hpp"

#include <gps.h>
#include <errno.h>

#include <iostream>
#include <cmath>


gpsd::gpsd()
{
    if ((gps_open("localhost", "2947", &gps_data_)) == -1) {
        std::cout << "code: " << errno << " reason: " <<  gps_errstr(errno) << '\n';
    }
}

gps_fix gpsd::run_until_fix()
{
    for(;;)
    {
        if ((gps_read(&gps_data_,NULL,0)) == -1) {
            std::cout << "error occured reading gps data. code: " << errno << " reason: " << gps_errstr(errno) << '\n';
        } else {
            if((gps_data_.status == STATUS_FIX) &&  (gps_data_.fix.mode == MODE_2D || gps_data_.fix.mode == MODE_3D) &&
                !std::isnan(gps_data_.fix.latitude) &&  !std::isnan(gps_data_.fix.longitude)) {
                    std::cout << "got fix\n";
                    gps_fix fix;
                    fix.latitude = gps_data_.fix.latitude;
                    fix.longitude = gps_data_.fix.longitude;
                    //fix.timestamp = (gps_data_.fix.time.tv_sec * 1000000ULL + gps_data_.fix.time.tv_nsec) * 1000;

                    return fix;
            } else {
                std::cout << "no GPS data available" << '\n';
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