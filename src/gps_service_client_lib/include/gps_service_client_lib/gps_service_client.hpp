/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>
#include <memory>

#include "gps_service.grpc.pb.h"

class GpsServiceClient {
public:
    GpsServiceClient(std::shared_ptr<grpc::Channel> channel);

    void StreamLocation(std::function<bool(const gps_service::StreamLocationResponse&)> func);

private:
    std::unique_ptr<gps_service::GpsService::Stub> stub_;
};

