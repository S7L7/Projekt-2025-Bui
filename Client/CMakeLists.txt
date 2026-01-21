cmake_minimum_required(VERSION 3.16)
project(attendance_client)

set(CMAKE_CXX_STANDARD 17)

add_executable(attendance_client
        main.cpp

        data/DataFetch.cpp
        data/Attendance_log.cpp
        data/ServerClient.cpp
        data/adminmenu.cpp


        amalgamations/sqlite3.c
)

target_include_directories(attendance_client
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib
)

target_link_libraries(attendance_client
        ws2_32
)
