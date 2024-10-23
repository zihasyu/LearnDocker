cmake_minimum_required(VERSION 3.10)
project(MyCacheProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 查找 Protobuf 和 gRPC 包
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 包含目录
include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS})

# 源文件
set(SOURCE_FILES
    src/main.cc
    src/cache_service.cc
    proto/cache.pb.cc
    proto/cache.grpc.pb.cc
)

# 生成可执行文件
add_executable(cache_server ${SOURCE_FILES})

# 链接库
target_link_libraries(cache_server ${Protobuf_LIBRARIES} ${gRPC_LIBRARIES})