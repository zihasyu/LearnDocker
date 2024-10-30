#include "cache.grpc.pb.h"
#include <grpcpp/grpcpp.h>
#include <iostream>
#include <memory>
#include <string>

class SDCSClient {
public:
    SDCSClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(SDCS::NewStub(channel)) {}

    std::string RPCGet(const std::string& key) {
        Key request;
        request.set_key(key);
        Value response;
        grpc::ClientContext context;

        grpc::Status status = stub_->RPCGet(&context, request, &response);
        if (status.ok()) {
            return response.value();
        } else {
            std::cout << "RPCGet failed: " << status.error_message() << std::endl;
            return "RPCGet failed";
        }
    }

    bool RPCPost(const std::string& key, const std::string& value) {
        Pair request;
        request.set_key(key);
        request.set_value(value);
        Flag response;
        grpc::ClientContext context;

        grpc::Status status = stub_->RPCPost(&context, request, &response);
        return status.ok();
    }

    bool RPCDelete(const std::string& key) {
        Key request;
        request.set_key(key);
        Flag response;
        grpc::ClientContext context;

        grpc::Status status = stub_->RPCDelete(&context, request, &response);
        return status.ok();
    }

private:
    std::unique_ptr<SDCS::Stub> stub_;
};

int main(int argc, char** argv) {
    SDCSClient client(grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));

    std::string key = "example_key";
    std::string value = "example_value";

    // RPCPost
    if (client.RPCPost(key, value)) {
        std::cout << "RPCPost succeeded" << std::endl;
    } else {
        std::cout << "RPCPost failed" << std::endl;
    }

    // RPCGet
    std::string response = client.RPCGet(key);
    std::cout << "RPCGet response: " << response << std::endl;

    // RPCDelete
    if (client.RPCDelete(key)) {
        std::cout << "RPCDelete succeeded" << std::endl;
    } else {
        std::cout << "RPCDelete failed" << std::endl;
    }

    return 0;
}