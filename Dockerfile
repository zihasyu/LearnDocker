# 使用基础镜像
FROM ubuntu:20.04

# 设置环境变量
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表
# 使用阿里云的源
RUN sed -i 's/archive.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list && \
    sed -i 's/security.ubuntu.com/mirrors.aliyun.com/g' /etc/apt/sources.list
RUN apt-get update

# 安装 build-essential
RUN apt-get install -y build-essential

# 安装 cmake
RUN apt-get install -y cmake

# 安装 protobuf-compiler
RUN apt-get install -y protobuf-compiler

# 安装 libprotobuf-dev
RUN apt-get install -y libprotobuf-dev

# 安装 libgRPC++-dev
RUN apt-get install -y libgRPC++-dev

# 安装 libssl-dev
RUN apt-get install -y libssl-dev

# 安装 libhttplib 库
RUN apt-get install -y  dos2unix curl net-tools

RUN apt install -y libgrpc++-dev
RUN apt install -y libgrpc++1
RUN apt install -y libgrpc-dev
RUN apt install -y libgrpc6
RUN apt install -y libyangrpc-dev
RUN apt install -y libyangrpc2
RUN apt install -y protobuf-compiler-grpc
RUN apt install -y dos2unix

# 清理
# RUN apt-get clean && rm -rf /var/lib/apt/lists/*



# 创建工作目录
WORKDIR /app

# 复制项目文件到容器
COPY . .

RUN dos2unix /app/setup.sh

# 运行 setup.sh 脚本
RUN bash setup.sh

# 设置工作目录到 build
WORKDIR /app/build

# 暴露 HTTP 服务端口
EXPOSE 5000

# # 设置默认命令
CMD ["./server"]
