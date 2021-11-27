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


#pragma once

#include <memory>
#include <iostream>
#include <string>
#include <thread>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#include "gps_service.grpc.pb.h"

class GpsService final : public gps_service::GpsService::Service
{
public:
  grpc::Status StreamLocation(grpc::ServerContext* ctx, 
                              const gps_service::StreamLocationRequest* request, 
                              grpc::ServerWriter<gps_service::StreamLocationResponse>* writer) override;
  
  static void run_server();
};
