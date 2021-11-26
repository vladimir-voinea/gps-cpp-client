#include <gps_service_client_lib/gps_service_client.hpp>

#include <stdlib.h>

int main(int argc, char** argv) {
    const auto server_host_env = ::getenv("GPS_SERVER");
    const std::string server_host = server_host_env ? server_host_env : "";

    const std::string server_string = server_host + ":50051";

    std::cout << "Server string: " + server_string + '\n';
    
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
    GreeterClient greeter(grpc::CreateChannel(server_string , grpc::InsecureChannelCredentials()));

    // Spawn reader thread that loops indefinitely
    std::thread thread_ = std::thread(&GreeterClient::AsyncCompleteRpc, &greeter);

    for (int i = 0; i < 100; i++) {
        std::string user("world " + std::to_string(i));
        greeter.SayHello(user);  // The actual RPC call!
    }

    std::cout << "Press control-c to quit" << std::endl << std::endl;
    thread_.join();  //blocks forever

    return 0;
}