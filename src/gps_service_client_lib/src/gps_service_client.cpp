#include <gps_service_client_lib/gps_service_client.hpp>

#include <iostream>

GpsServiceClient::GpsServiceClient(std::shared_ptr<grpc::Channel> channel)
    : stub_(gps_service::GpsService::NewStub(channel))
{}

void GpsServiceClient::StreamLocation(std::function<void(gps_service::StreamLocationResponse)> func)
{
    grpc::ClientContext ctx;
    gps_service::StreamLocationRequest req;
    gps_service::StreamLocationResponse res;

    std::unique_ptr<grpc::ClientReader<gps_service::StreamLocationResponse>> reader(stub_->StreamLocation(&ctx, req));

    while(reader->Read(&res)) {
        func(res);
    }

    grpc::Status status = reader->Finish();
    if(status.ok()) {
        std::cout << "StreamLocation rpc succeeded" << std::endl;
    } else {
        std::cout << "StreamLocation rpc failed" << std::endl;
    }
}
