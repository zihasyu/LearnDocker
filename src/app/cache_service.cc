#include "cache.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <unordered_map>
#include <mutex>

class CacheServiceImpl final : public CacheService::Service
{
public:
  grpc::Status GetKey(grpc::ServerContext *context, const KeyRequest *request, KeyValue *response) override
  {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = cache_.find(request->key());
    if (it != cache_.end())
    {
      response->set_key(it->first);
      response->set_value(it->second);
      return grpc::Status::OK;
    }
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Key not found");
  }

  grpc::Status SetKey(grpc::ServerContext *context, const KeyValue *request, Status *response) override
  {
    std::lock_guard<std::mutex> lock(mutex_);
    cache_[request->key()] = request->value();
    response->set_success(true);
    return grpc::Status::OK;
  }

  grpc::Status DeleteKey(grpc::ServerContext *context, const KeyRequest *request, Status *response) override
  {
    std::lock_guard<std::mutex> lock(mutex_);
    response->set_success(cache_.erase(request->key()) > 0);
    return grpc::Status::OK;
  }

private:
  std::unordered_map<std::string, std::string> cache_;
  std::mutex mutex_;
};