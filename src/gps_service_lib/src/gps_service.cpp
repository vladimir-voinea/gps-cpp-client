#include <gps_service_lib/gps_service.hpp>

grpc::Status GpsService::StreamLocation(grpc::ServerContext* ctx, 
                            const gps_service::StreamLocationRequest* request, 
                            grpc::ServerWriter<gps_service::StreamLocationResponse>* writer)
{
    for(auto i = 0u; i < 100; i++)
    {
        gps_service::Point point;
        point.set_latitude(static_cast<double>(i));
        point.set_longitude(static_cast<double>(i));

        gps_service::StreamLocationResponse res;
        res.mutable_point()->CopyFrom(point);

        writer->Write(res);
    }
    
    return grpc::Status::OK;
}

void GpsService::run_server()
{
    const std::string server_address = "0.0.0.0:50051";

    GpsService service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}
