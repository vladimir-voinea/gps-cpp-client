#include <gps_service_lib/gps_service.hpp>

#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
  try
  {
    GpsService::run_server();
  }
  catch(const std::exception& e)
  {
    spdlog::error("Exception: {}",e.what());
  }
  

  return 0;
}