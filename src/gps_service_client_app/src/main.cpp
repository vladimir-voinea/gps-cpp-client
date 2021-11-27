#include <gps_service_client_lib/gps_service_client.hpp>

#include <stdlib.h>

int main(int argc, char** argv) {
    const auto server_host_env = ::getenv("GPS_SERVER");
    const std::string server_host = server_host_env ? server_host_env : "";

    const std::string server_string = server_host + ":50051";

    std::cout << "Server string: " + server_string + '\n';

    GpsServiceClient client(grpc::CreateChannel(server_string, grpc::InsecureChannelCredentials()));

    client.StreamLocation([](auto res) {
        std::cout << res.point().latitude() << ' ' << '\n';
    });

    return 0;
}