cmake_minimum_required(VERSION 4.0)
project(Dochazkovy_system)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dochazkovy_system
        main.cpp
        sqlite3.c)

target_link_libraries(Dochazkovy_system
        PRIVATE
        Threads::Threads
)
