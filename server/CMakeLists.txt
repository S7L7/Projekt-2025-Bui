cmake_minimum_required(VERSION 3.16)
project(attendance_server)

set(CMAKE_CXX_STANDARD 17)

add_executable(attendance_server
        server.cpp
        amalgamations/sqlite3.c
)

target_include_directories(attendance_server PRIVATE
        cpp-httplib
        amalgamations
)

target_link_libraries(attendance_server
        ws2_32
)
