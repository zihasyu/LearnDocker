#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <unordered_map>
#include <iostream>

std::unordered_map<std::string, web::json::value> cache;

void handle_post(web::http::http_request request) {
    request.extract_json().then([request](web::json::value body) {
        auto it = body.as_object().begin();
        std::string key = it->first;
        cache[key] = it->second;
        request.reply(web::http::status_codes::OK, body);
    });
}

void handle_get(web::http::http_request request) {
    auto path = web::uri::split_path(request.relative_uri().path());
    std::string key = path[0];
    if (cache.find(key) != cache.end()) {
        request.reply(web::http::status_codes::OK, cache[key]);
    } else {
        request.reply(web::http::status_codes::NotFound);
    }
}

void handle_delete(web::http::http_request request) {
    auto path = web::uri::split_path(request.relative_uri().path());
    std::string key = path[0];
    if (cache.erase(key)) {
        request.reply(web::http::status_codes::OK, "1");
    } else {
        request.reply(web::http::status_codes::OK, "0");
    }
}

int main() {
    web::http::experimental::listener::http_listener listener("http://0.0.0.0:5000");
    listener.support(web::http::methods::POST, handle_post);
    listener.support(web::http::methods::GET, handle_get);
    listener.support(web::http::methods::DEL, handle_delete);

    try {
        listener
            .open()
            .then([&listener]() { std::cout << "Starting server...\n"; })
            .wait();
        while (true);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}