//
// Created by Beast on 20.01.2026.
//
#define _WIN32_WINNT 0x0A00
#include <winsock2.h>

#include <iostream>
#include "cpp-httplib/httplib.h"

int main() {
    httplib::Server server;

    server.Get("/", [](const httplib::Request&, httplib::Response& res) {
        std::cout << std::endl << "user has joined!" << std::endl;
        res.set_content("Server is online!", "text/plain");
    });
    std::cout << "Port 8080";
    server.listen("0.0.0.0", 8080);
}

// K Připojení využíj http://"ipconfig ip":8080