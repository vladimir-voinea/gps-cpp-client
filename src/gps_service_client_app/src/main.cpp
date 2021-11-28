#include <gps_service_client_lib/gps_service_client.hpp>
#include <spdlog/spdlog.h>

#include <stdlib.h>

int main(int argc, char** argv) {
    const auto server_host_env = ::getenv("GPS_SERVER");
    const std::string server_host = server_host_env ? server_host_env : "";

    const std::string server_string = server_host + ":50051";

    spdlog::info("Server string: {}", server_string);

    GpsServiceClient client(grpc::CreateChannel(server_string, grpc::InsecureChannelCredentials()));

    client.StreamLocation([](auto res) {
        spdlog::info("Latitude: {} Longitude: {}", res.point().latitude(), res.point().longitude());
        return true;
    });

    return 0;
}