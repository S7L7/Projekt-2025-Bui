cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)
project(Projekt2025-Bui)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)

add_executable(Projekt2025-Bui
        main.cpp
        sqlite3.c
)

target_link_libraries(Projekt2025-Bui
        PRIVATE
        Threads::Threads
)