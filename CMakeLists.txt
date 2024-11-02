cmake_minimum_required(VERSION 3.10)
project(SDCS)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 Protobuf 包
find_package(Protobuf REQUIRED)

# 手动设置 gRPC 的包含路径和库路径
include_directories(/usr/include/grpcpp)
include_directories(/usr/include)
link_directories(/usr/lib /usr/local/lib)

# 包含头文件目录
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/proto)
include_directories(/usr/include/nlohmann)

# 源文件列表
set(SERVER_SOURCES
    src/app/server_main.cc
    src/app/cache_service.cc
    src/proto/cache.pb.cc
    src/proto/cache.grpc.pb.cc
)

set(CLIENT_SOURCES
    src/app/client_main.cc
    src/proto/cache.pb.cc
    src/proto/cache.grpc.pb.cc
)

# 添加可执行文件
add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})

# 链接库
target_link_libraries(server protobuf grpc++ grpc pthread )
target_link_libraries(client protobuf grpc++ grpc pthread )