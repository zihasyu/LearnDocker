#ifndef CACHE_SERVICE_H
#define CACHE_SERVICE_H

#include <string>
#include <unordered_map>
#include <mutex>
#include "cache.grpc.pb.h"

class SDCSImpl final : public SDCS::Service
{
public:
  grpc::Status RPCGet(grpc::ServerContext *context, const Key *request, Value *response) override;
  grpc::Status RPCPost(grpc::ServerContext *context, const Pair *request, Flag *response) override;
  grpc::Status RPCDelete(grpc::ServerContext *context, const Key *request, Flag *response) override;
  grpc::Status RPCAddNextServer(grpc::ServerContext *context, const Flag *request, Flag *response) override;

private:
  std::unordered_map<std::string, std::string> cache_;
  std::mutex cache_mutex_;
};

#endif // CACHE_SERVICE_H