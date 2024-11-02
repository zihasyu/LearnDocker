void setup_http_routes(Server &svr, CacheServiceImpl &service, const std::vector<std::string> &grpc_addresses)
{
    svr.Post("/", [&](const Request &req, Response &res)
             {
        json jsonData = json::parse(req.body);
        for (auto& [key, value] : jsonData.items()) {
            int node_index = std::stoi(hashFunction(key));  // 根据哈希选择节点
            std::string target_address = grpc_addresses[node_index];

            // 使用 gRPC 将数据存储到对应节点
            grpc::ClientContext context;
            cache::KeyValueRequest grpc_request;
            grpc_request.set_key(key);
            grpc_request.set_value(value);
            cache::EmptyResponse grpc_response;

            auto grpc_stub = cache::CacheService::NewStub(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
            grpc::Status status = grpc_stub->SetValue(&context, grpc_request, &grpc_response);

            if (!status.ok()) {
                std::cerr << "gRPC call failed: " << status.error_message() << std::endl;
            }
        }
        res.set_content("{}", "application/json");
        res.status = 200; });

    svr.Get(R"(/(.+))", [&](const Request &req, Response &res)
            {
        std::string key = req.matches[1];
        int node_index = std::stoi(hashFunction(key));  // 根据哈希选择节点
        std::string target_address = grpc_addresses[node_index];

        // 使用 gRPC 从对应节点获取数据
        grpc::ClientContext context;
        cache::KeyRequest grpc_request;
        grpc_request.set_key(key);
        cache::KeyValueResponse grpc_response;

        auto grpc_stub = cache::CacheService::NewStub(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
        grpc::Status status = grpc_stub->GetValue(&context, grpc_request, &grpc_response);

        if (status.ok()) {
            json response = {{grpc_response.key(), grpc_response.value()}};
            res.set_content(response.dump(), "application/json");
            res.status = 200;
        } else {
            res.status = 404;
            res.set_content("", "application/json");
        } });

    svr.Delete(R"(/(.+))", [&](const Request &req, Response &res)
               {
        std::string key = req.matches[1];
        int node_index = std::stoi(hashFunction(key));  // 根据哈希选择节点
        std::string target_address = grpc_addresses[node_index];

        // 使用 gRPC 从对应节点删除数据
        grpc::ClientContext context;
        cache::KeyRequest grpc_request;
        grpc_request.set_key(key);
        cache::DeleteResponse grpc_response;

        auto grpc_stub = cache::CacheService::NewStub(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()));
        grpc::Status status = grpc_stub->DeleteValue(&context, grpc_request, &grpc_response);

        json response = {{"deleted_count", grpc_response.deleted_count()}};
        res.set_content(response.dump(), "application/json");
        res.status = 200; });
}