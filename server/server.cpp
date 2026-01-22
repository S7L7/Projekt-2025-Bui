//
// Created by Beast on 20.01.2026.
//
#define _WIN32_WINNT 0x0A00
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ctime>
#include <iostream>
#include <../amalgamations/sqlite3.h>
#include "cpp-httplib/httplib.h"
#include <string>


sqlite3* openDatabase() {
    sqlite3* db = nullptr;

        if (sqlite3_open("C:/Users/Beast/Desktop/Projekt-2025-Bui/server/attendance_server.db",&db
        )!= SQLITE_OK){
            std::cerr << "nelze otevrit server db" << std::endl;
            return nullptr;
        }
    return db;
    }
bool insertAttendance(sqlite3* db, const std::string& rfid,const std::string& type) {
    const char* sql = "INSERT into attendance (rfid, type, timestamp) VALUES (?,?,?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "SQL error (prepare):"<< sqlite3_errmsg(db) << std::endl;
        return false;
    }
    //timestamp
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[20];
    strftime(buffer,sizeof(buffer), "%Y-%m-%d %H:%M", ltm);

    sqlite3_bind_text(stmt, 1, rfid.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, type.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, buffer, -1, SQLITE_TRANSIENT);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    if (!ok) {
        std::cerr << "SQL error (step): "
                  << sqlite3_errmsg(db) << std::endl;
    }
        sqlite3_finalize(stmt);
        return ok;
}
int main() {
    httplib::Server server;
    sqlite3* db = openDatabase();
    if (!db) {
        std::cerr << "DB init failed, server konci" << std::endl;
        return 1;
    }

    server.Post("/punch", [&](const httplib::Request& req, httplib::Response& res) {
        std::cout << std::endl << "==== PRISLA DATA HALLELUJA ====>" << req.body << std::endl;

        auto rfidPos = req.body.find("\"rfid\":\"");
        auto typePos = req.body.find("\"type\":\"");

        if (rfidPos == std::string::npos || typePos == std::string::npos) {
            res.status = 400;
            return;
        }
        std::string rfid = req.body.substr(rfidPos + 8, req.body.find("\"", rfidPos + 8) - (rfidPos + 8));
        std::string type = req.body.substr(typePos + 8, req.body.find("\"", typePos + 8) - (typePos + 8));


        if (insertAttendance(db,rfid,type)) {
            res.set_content("OK", "text/plain");
        } else {
            res.status = 500;
        }
        std::cout << "PARSED RFID: " << rfid << std::endl;
        std::cout << "PARSED TYPE: " << type << std::endl;
    });
    std::cout << "Port 8080";
    server.listen("0.0.0.0", 8080);
}
// K Připojení tutaj http://"ipconfig ip":8080