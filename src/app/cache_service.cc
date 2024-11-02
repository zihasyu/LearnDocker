#include "cache_service.h"

grpc::Status SDCSImpl::RPCPost(grpc::ServerContext* context, const Pair* request, Flag* response) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    cache_[request->key()] = request->value();
    response->set_flag(1);
    return grpc::Status::OK;
}

grpc::Status SDCSImpl::RPCGet(grpc::ServerContext* context, const Key* request, Value* response) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    auto it = cache_.find(request->key());
    if (it != cache_.end()) {
        response->set_value(it->second);
        return grpc::Status::OK;
    } else {
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Key not found");
    }
}

grpc::Status SDCSImpl::RPCDelete(grpc::ServerContext* context, const Key* request, Flag* response) {
    std::lock_guard<std::mutex> lock(cache_mutex_);
    int erased = cache_.erase(request->key());
    response->set_flag(erased);
    return grpc::Status::OK;
}

grpc::Status SDCSImpl::RPCAddNextServer(grpc::ServerContext* context, const Flag* request, Flag* response) {
    // Implementation for adding the next server (not provided)
    response->set_flag(1);
    return grpc::Status::OK;
}
//curl -XDELETE http://127.0.0.1:9527/myname
//curl http://127.0.0.1:9527/myname
//curl -XPOST -H "Content-type: application/json" http://127.0.0.1:9527/ -d '{"myname": "电子科技大学@2024"}'

