#include <gps_service_lib/gps_service.hpp>

int main(int argc, char** argv) {
  ServerImpl server;
  server.Run();

  return 0;
}