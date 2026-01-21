//
// Created by buijo on 30.12.2025.
//
#include "../Attendance_log.h"
#include "../ServerClient.h"
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

static string getCurrentTimeStamp() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", ltm);
    return string(buffer);
}
void writeAttendanceToFile(const string& employeeName, const string& type, const string& rfid) {
    ofstream file("attendance_log.txt", ios::app);

    if (!file.is_open()) {
        cerr << "Can't open attendance_log.txt" << endl;
        return;
    }

    file <<getCurrentTimeStamp()
         << " | "
         << employeeName
         << " | "
         << type
         << endl;
    if (!sendtoServer(rfid,type)) {
        std::cerr << "Nepodarilo se odeslat data na server" << std::endl;
    }

    file.close();
}