#include <grpcpp/grpcpp.h>
#include "cache.grpc.pb.h"
#include <iostream>
#include <string>

class CacheServiceImpl final : public CacheService::Service
{
  // Implement gRPC methods here
};

void RunServer(const std::string &server_address)
{
  CacheServiceImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char **argv)
{
  std::string server_address("0.0.0.0:50051");
  RunServer(server_address);
  return 0;
}