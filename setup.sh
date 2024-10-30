#!/bin/bash

# 删除旧的构建目录并创建新的构建目录
rm -rf build
mkdir build
cd build

# 运行 CMake 配置
cmake ..

# 编译项目
make