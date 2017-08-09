cmake_minimum_required(VERSION 3.3)

project(libuv_example)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# source files
set(
        SOURCE_FILES
        ./src/main.c
)

# execute
add_executable(libuv_example ${SOURCE_FILES})

# link file
# pkg-config --cflags --libs libuv
target_link_libraries(libuv_example -I/usr/local/include -L/usr/local/lib -luv -lpthread -ldl)
