//
// Created by bui on 29.11.2025.
//
#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <unordered_map>
#include "DataFetch.h"
#include "Attendance_log.h"
#include "adminmenu.h"

using namespace std;
using Clock = std::chrono::steady_clock;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8); // pro změnu na UTF-8 chcp 65001

    sqlite3* db = openDatabase("../attendance.db");
    if (!db) return 1;

    std::unordered_map<int,Clock::time_point> lastValidAction;

    while (true) {
        string rfid;
        cout << "Zadejte ID: ";
        cin >> rfid;
        if (rfid == "ADMINMODE") {
            runAdminMenu(db);
            continue;
        }
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

        auto it = lastValidAction.find(emp.id);
        if (it != lastValidAction.end()) {
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - it->second).count();

            if (diff < 10) {
                    cerr << "Neplatný vstup(počkejte 10 vteřin)" << endl;
                    continue;
                }
            }

        string type;
        bool isEntry = (emp.status == 0);
        type = isEntry ? "entry" : "exit";
        cout << "Entry/Exit: " << type << endl;

        if (insertAttendance(db, emp.id, type)) {
            int newStatus = isEntry ? 1 : 0;

            if (updateEmployeeStatus(db, emp.id, newStatus)) {
                lastValidAction[emp.id] = now;
            }

            cout << "Dochazka zapsana" << endl;

            writeAttendanceToFile(
    emp.name,
           emp.rfid,
            type);}else {
                        cout << "Error: Nezapsano" << endl;
            }
    }
    sqlite3_close(db);
    return 0;
}