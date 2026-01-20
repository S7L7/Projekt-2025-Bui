//
// Created by Beast on 20.01.2026.
//
#define _WIN32_WINNT 0x0A00
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>
#include "../server/cpp-httplib/httplib.h"
#include "../Client/ServerClient.h"

bool sendtoServer(const std::string& rfid, const std::string& type) {
    httplib::Client client("localhost", 8080);

    std::string json =
        "{"
        "\"rfid\":\"" + rfid + "\","
        "\"type\":\"" + type + "\","
        "}";
    auto res = client.Post("/punch",
        json,
        "application/json");

   if (!res) {
       std::cerr << "Server je nedostupný" << std::endl;
       return false;
   }
   if (res -> status != 200) {
       std::cerr << "Server vrátil chybu: " << res -> status << std::endl;
       return false;
   }
    return true;
}
