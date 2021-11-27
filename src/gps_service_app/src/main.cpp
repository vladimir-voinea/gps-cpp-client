#include <gps_service_lib/gps_service.hpp>

int main(int argc, char** argv) {
  try
  {
    GpsService::run_server();
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  

  return 0;
}