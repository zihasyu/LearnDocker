#include <httplib.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <functional>

std::unordered_map<std::string, std::string> cache;
std::mutex cache_mutex;

std::vector<std::string> servers = {"http://127.0.0.1:9527", "http://127.0.0.1:9528", "http://127.0.0.1:9529"};

size_t hash_key(const std::string& key) {
    std::hash<std::string> hasher;
    return hasher(key) % servers.size();
}

std::string get_server_for_key(const std::string& key) {
    size_t index = hash_key(key);
    return servers[index];
}

void RunHealthCheckServer() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("OK", "text/plain");
        res.status = 200;
    });

    svr.Post("/", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.get_param_value("key");
        auto value = req.get_param_value("value");
        std::string server = get_server_for_key(key);
        if (server == "http://127.0.0.1:5000") {
            {
                std::lock_guard<std::mutex> lock(cache_mutex);
                cache[key] = value;
            }
            res.set_content("OK", "text/plain");
            res.status = 200;
        } else {
            httplib::Client cli(server.c_str());
            auto response = cli.Post("/", "key=" + key + "&value=" + value, "application/x-www-form-urlencoded");
            if (response && response->status == 200) {
                res.set_content("OK", "text/plain");
                res.status = 200;
            } else {
                res.set_content("Error", "text/plain");
                res.status = 500;
            }
        }
    });

    svr.Get(R"(/(.*))", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.matches[1].str();
        std::string server = get_server_for_key(key);
        if (server == "http://127.0.0.1:5000") {
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
                res.status = 200;
            } else {
                res.status = 404;
            }
        } else {
            httplib::Client cli(server.c_str());
            auto response = cli.Get(("/" + key).c_str());
            if (response && response->status == 200) {
                res.set_content(response->body, "text/plain");
                res.status = 200;
            } else {
                res.set_content("Error", "text/plain");
                res.status = 500;
            }
        }
    });

    svr.Delete(R"(/(.*))", [](const httplib::Request& req, httplib::Response& res) {
        auto key = req.matches[1].str();
        std::string server = get_server_for_key(key);
        if (server == "http://127.0.0.1:5000") {
            int deleted = 0;
            {
                std::lock_guard<std::mutex> lock(cache_mutex);
                auto it = cache.find(key);
                if (it != cache.end()) {
                    cache.erase(it);
                    deleted = 1;
                }
            }
            res.set_content(std::to_string(deleted), "text/plain");
            res.status = 200;
        } else {
            httplib::Client cli(server.c_str());
            auto response = cli.Delete(("/" + key).c_str());
            if (response && response->status == 200) {
                res.set_content(response->body, "text/plain");
                res.status = 200;
            } else {
                res.set_content("Error", "text/plain");
                res.status = 500;
            }
        }
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
    std::thread http_server_thread(RunHealthCheckServer);
    RunGRPCServer();
    http_server_thread.join();
    return 0;
}