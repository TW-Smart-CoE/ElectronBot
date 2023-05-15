#!/bin/bash

# 创建 build 目录 
if [ ! -d "build" ]; then 
    mkdir build 
fi 

# 进入 build 目录
cd build 

# 执行 conan install
# conan install ..
# conan install ../conanfile.txt 
conan install ../conanfile.txt --build=missing --output-folder=conaninfo

# 执行 cmake
# CMAKE_PREFIX_PATH="../" cmake -DCMAKE_BUILD_TYPE=Release .. 
CMAKE_PREFIX_PATH="./conaninfo" cmake -DCMAKE_BUILD_TYPE=Release .. 

# 执行 make
make