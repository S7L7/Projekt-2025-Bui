cmake_minimum_required(VERSION 3.16)
project(attendance_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(attendance_server
        server.cpp
)

target_link_libraries(attendance_server
        ws2_32
)