FROM ubuntu:20.04

RUN apt-get update && \
    apt-get install -y build-essential cmake libssl-dev \
    libprotobuf-dev protobuf-compiler \
    grpc libgrpc++-dev && \
    apt-get clean

COPY . /app
WORKDIR /app

RUN mkdir build && cd build && cmake .. && make

ENTRYPOINT ["./build/cache_server"]