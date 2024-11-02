#include "../../include/cache_service.h"
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <thread>
#include <httplib.h>
#include <unordered_map>
#include <mutex>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
std::unordered_map<std::string, std::string> cache;
std::mutex cache_mutex;
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
                std::lock_guard<std::mutex> lock(cache_mutex);
                cache[key] = value;
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
        std::lock_guard<std::mutex> lock(cache_mutex);
        if (cache.find(key) != cache.end()) {
            json j = { { key, cache[key] } };
            res.set_content(j.dump(), "application/json");
            res.status = 200;
        } else {
            res.set_content("Key not found", "text/plain");
            res.status = 404;
        }
    });

    svr.Delete(R"(/(.*))", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.matches[1].str();
        std::lock_guard<std::mutex> lock(cache_mutex);
        if (cache.erase(key)) {
            res.set_content("1", "text/plain");
        } else {
            res.set_content("0", "text/plain");
        }
        res.status = 200;
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