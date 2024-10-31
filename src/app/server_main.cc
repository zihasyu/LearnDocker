#include "../../include/cache_service.h"
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <thread>
#include <httplib.h>
#include <unordered_map>
#include <mutex>

std::unordered_map<std::string, std::string> cache;
std::mutex cache_mutex;

void RunHealthCheckServer() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
    });

    svr.Post("/set", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");
        {
            std::lock_guard<std::mutex> lock(cache_mutex);
            cache[key] = value;
        }
        res.set_content("OK", "text/plain");
    });

    svr.Get("/get", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        std::string value;
        {
            std::lock_guard<std::mutex> lock(cache_mutex);
            auto it = cache.find(key);
            if (it != cache.end()) {
                value = it->second;
            }
        }
        if (!value.empty()) {
            res.set_content(value, "text/plain");
        } else {
            res.status = 404;
        }
    });

    svr.Post("/delete", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        {
            std::lock_guard<std::mutex> lock(cache_mutex);
            cache.erase(key);
        }
        res.set_content("OK", "text/plain");
    });

    svr.listen("0.0.0.0", 5000);
}

void RunGRPCServer() {
    std::string server_address("0.0.0.0:50051");
    SDCSImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}

int main(int argc, char** argv) {
    std::thread health_check_thread(RunHealthCheckServer);
    RunGRPCServer();
    health_check_thread.join();
    return 0;
}