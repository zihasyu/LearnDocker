#include "../../include/cache_service.h"
#include <grpcpp/grpcpp.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <thread>
#include <httplib.h>
#include <unordered_map>
#include <mutex>
#include <nlohmann/json.hpp>
#include "cache.grpc.pb.h"
#include <cstdlib>

using json = nlohmann::json;
// std::mutex cache_mutex;
SDCSImpl service;
int HTTP_PORT = 5000;
int GRPC_PORT = 50050;
int SERVER_INDEX = 0;
int cs_num = 3; // 假设有3个服务器

int GetServerIndex(const std::string& key) {
    std::hash<std::string> hash_fn;
    return hash_fn(key) % cs_num;
}

void SyncDataWithServer(const std::string& key, const std::string& value, int server_index) {
    std::string server_address = "cache-server-" + std::to_string(server_index + 1) + ":" + std::to_string(GRPC_PORT + server_index);
    std::cout << "Syncing data to server: " << server_address << std::endl;

    auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
    std::unique_ptr<SDCS::Stub> stub = SDCS::NewStub(channel);

    Pair pair;
    pair.set_key(key);
    pair.set_value(value);

    Flag response;
    grpc::ClientContext context;
    grpc::Status status = stub->RPCPost(&context, pair, &response);
    if (status.ok()) {
        std::cout << "Successfully synced data to server: " << server_address << std::endl;
    } else {
        std::cerr << "Failed to sync data with server " << server_address << ": " << status.error_message() << std::endl;
    }
}

void RunHealthCheckServer() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
        res.status = 200;
    });

    svr.Post("/", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto j = json::parse(req.body);
            for (auto& [key, value] : j.items()) {
                int server_index = GetServerIndex(key);
                if (server_index == SERVER_INDEX) {
                    // std::lock_guard<std::mutex> lock(cache_mutex);
                    service.cache_[key] = value;
                    std::cout<<"flag1 "<<key<<":"<<value<<std::endl;
                } else {
                    SyncDataWithServer(key, value, server_index);
                }
            }
            res.set_content("OK", "text/plain");
            res.status = 200;
        } catch (const json::parse_error& e) {
            res.set_content("Invalid JSON", "text/plain");
            res.status = 400;
        }
    });

    svr.Get(R"(/(.*))", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.matches[1].str();
        int server_index = GetServerIndex(key);
        if (server_index == SERVER_INDEX) {
            // std::lock_guard<std::mutex> lock(cache_mutex);

            // std::cout<<key<<std::endl;
            // for(auto& [key, value]:cache){
            //     std::cout<<key<<":"<<value<<std::endl;
            // }

            if (service.cache_.find(key) != service.cache_.end()) {
                json j = { { key, service.cache_[key] } };
                res.set_content(j.dump(), "application/json");
                res.status = 200;
            } else {
                res.set_content("Key not found", "text/plain");
                res.status = 404;
            }
        } else {
            std::string server_address = "cache-server-" + std::to_string(server_index + 1) + ":" + std::to_string(GRPC_PORT + server_index);
            auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
            std::unique_ptr<SDCS::Stub> stub = SDCS::NewStub(channel);

            Key request;
            request.set_key(key);
            Value response;
            grpc::ClientContext context;
            grpc::Status status = stub->RPCGet(&context, request, &response);

            // std::cout<<key<<std::endl;
            // for(auto& [key, value]:cache){
            //     std::cout<<key<<":"<<value<<std::endl;
            // }

            if (status.ok()) {
                json j = { { key, response.value() } };
                res.set_content(j.dump(), "application/json");
                res.status = 200;
            } else {
                std::cerr << "Failed to get data from server " << server_address << ": " << status.error_message() << std::endl;
                res.set_content("Key not found", "text/plain");
                res.status = 404;
            }
        }
    });

    svr.Delete(R"(/(.*))", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.matches[1].str();
        int server_index = GetServerIndex(key);
        if (server_index == SERVER_INDEX) {
            // std::lock_guard<std::mutex> lock(cache_mutex);
            if (service.cache_.erase(key)) {
                res.set_content("1", "text/plain");
            } else {
                res.set_content("0", "text/plain");
            }
            res.status = 200;
        } else {
            std::string server_address = "cache-server-" + std::to_string(server_index + 1) + ":" + std::to_string(GRPC_PORT + server_index);
            auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
            std::unique_ptr<SDCS::Stub> stub = SDCS::NewStub(channel);

            Key request;
            request.set_key(key);
            Flag response;
            grpc::ClientContext context;
            grpc::Status status = stub->RPCDelete(&context, request, &response);

            if (status.ok() && response.flag() == 1) {
                res.set_content("1", "text/plain");
            } else {
                std::cerr << "Failed to delete data from server " << server_address << ": " << status.error_message() << std::endl;
                res.set_content("0", "text/plain");
            }
            res.status = 200;
        }
    });

    svr.listen("0.0.0.0", HTTP_PORT);
}

void RunGRPCServer() {
    std::string server_address = "0.0.0.0:" + std::to_string(GRPC_PORT+SERVER_INDEX);
    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main(int argc, char** argv) {



    const char* server_index_env = std::getenv("SERVER_INDEX");
    if (server_index_env) {
        SERVER_INDEX = std::stoi(server_index_env);
    }

    std::thread health_check_server(RunHealthCheckServer);
    std::thread grpc_server(RunGRPCServer);
    health_check_server.join();
    grpc_server.join();
    return 0;
}