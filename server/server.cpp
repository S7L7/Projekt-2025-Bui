//
// Created by Beast on 20.01.2026.
//
#define _WIN32_WINNT 0x0A00
#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include "cpp-httplib/httplib.h"

int main() {
    httplib::Server server;

    server.Post("/punch", [](const httplib::Request& req, httplib::Response& res) {
        std::cout << std::endl << "==== PRISLA DATA HALLELUJA ====" << std::endl;
        std::cout << req.body << std::endl;
        std::cout << ("===============================", "text/plain");

        res.set_content("OK", "text/plain");
    });
    std::cout << "Port 8080";
    server.listen("0.0.0.0", 8080);
}

// K Připojení využíj http://"ipconfig ip":8080