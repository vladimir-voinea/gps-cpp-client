#include <gps_service_lib/gps_service.hpp>

#include "gpsd.hpp"

#include <spdlog/spdlog.h>

grpc::Status GpsService::StreamLocation(grpc::ServerContext* ctx, 
                            const gps_service::StreamLocationRequest* request, 
                            grpc::ServerWriter<gps_service::StreamLocationResponse>* writer)
{   
    gpsd gps;

    bool go_on = gps.good();
    while(go_on)
    {    
        const auto fix = gps.run_until_fix();

        gps_service::StreamLocationResponse res;
        res.mutable_point()->set_timestamp(fix.timestamp);
        res.mutable_point()->set_latitude(fix.latitude);
        res.mutable_point()->set_longitude(fix.longitude);

        if(fix.altitude) {
            res.mutable_point()->set_altitude(*fix.altitude);
        }

        if(fix.bearing) {
            res.mutable_point()->set_bearing(*fix.bearing);
        }

        if(fix.speed) {
            res.mutable_point()->set_speed(*fix.speed);
        }

        if(fix.climb) {
            res.mutable_point()->set_climb(*fix.climb);
        }

        go_on = writer->Write(res);
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

    spdlog::info("Server listening on {}", server_address);

    server->Wait();

    spdlog::info("Done wait");
}
