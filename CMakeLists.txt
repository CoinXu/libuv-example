cmake_minimum_required(VERSION 3.3)
project(libuv_example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    src/main.c
    CMakeLists.txt
    docker.sh
    Dockerfile
    Makefile
    sources.list)

add_executable(libuv_example ${SOURCE_FILES})