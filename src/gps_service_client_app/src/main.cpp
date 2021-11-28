#include <gps_service_client_lib/gps_service_client.hpp>
#include <spdlog/spdlog.h>

#include <stdlib.h>

int main(int argc, char** argv) {
    const auto server_host_env = ::getenv("GPS_SERVER");
    const std::string server_host = server_host_env ? server_host_env : "";

    const std::string server_string = server_host + ":50051";

    spdlog::info("Server string: {}", server_string);

    GpsServiceClient client(grpc::CreateChannel(server_string, grpc::InsecureChannelCredentials()));

    int fixes = 100;
    client.StreamLocation([fixes](auto res) mutable {
        spdlog::info("Latitude: {} Longitude: {}", res.point().latitude(), res.point().longitude());
        fixes--;
        return fixes;
    });

    return 0;
}