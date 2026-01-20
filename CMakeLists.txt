cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)
project(Projekt2025-Bui)

find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
#Píchačka exe
add_executable(attendance
        main.cpp
        amalgamations/sqlite3.c
        data/DataFetch.cpp
        data/Attendance_log.cpp
        data/adminmenu.cpp
        att_record.h

)
target_link_libraries(attendance
        PRIVATE
        Threads::Threads
)

add_executable(attendance_server
        server/server.cpp
)

target_link_libraries(attendance_server
PRIVATE ws2_32 Threads::Threads
)