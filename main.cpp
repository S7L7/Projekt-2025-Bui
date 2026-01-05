//
// Created by bui on 29.11.2025.
//
#include <iostream>
#include <string>
#include "DataFetch.h"
#include "Attendance_log.h"
#include <windows.h>
#include <chrono>
#include <unordered_map>

using namespace std;
using Clock = std::chrono::steady_clock;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // chcp 65001

        sqlite3* db = openDatabase("../attendance.db");
        if (!db) return 1;
    std::unordered_map<int,Clock::time_point> LastValidAction;
    while (true) {
        string rfid;
        cout << "Zadejte ID: ";
        cin >> rfid;

        if (rfid == "EXIT") {
            break;
        }

        Employee emp = getEmployeeByRfid(db, rfid);

        if (emp.id == -1) {
            cout << "Zamestnanec nenalezen" << endl;
            continue;
        }

        cout << emp.id << " | " << emp.name << endl;

        auto now = Clock::now();

        auto it = LastValidAction.find(emp.id);
        if (it != LastValidAction.end()) {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second).count();
            if (diff < 10) {
                cerr << "Neplatný vstup(počkejte 10 vteřin)" << endl;
                continue;
            }
        }
        auto now = Clock::now();

        auto it = lastValidAction.find(emp.id);
        if (it != lastValidAction.end()) {
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count();

            if (diff < 10) {
                std::cerr << "Invalid log (too fast)" << std::endl;
                continue; // NIC se nezapíše, stav se nemění
            }
        }

        string type;
        if (emp.status == 0) {
            type = "Entry";
        }else {
            type = "Exit";
        }
        auto LastValid = getCurrentTime();
        std::cout << "Start Time: " << startTime.time_since_epoch().count() << "\n";
        std::cout << (getCurrentTime() - startTime).count() << "\n";
    }
        cout << "Entry/Exit" << type << endl;
        string employeeName = getEmployeeName(db, emp.id);

        if (insertAttendance(db, emp.id, type)) {
            int newStatus = (type == "entry") ? 1:0;
            updateEmployeeStatus(db, emp.id, newStatus);

            cout << "Dochazka zapsana" << endl;

            writeAttendanceToFile(
            to_string(emp.id)+ " | " + employeeName,
                type);
        } else {
            cout << "Error: Nezapsano" << endl;
        }
    }
        sqlite3_close(db);
        return 0;
}