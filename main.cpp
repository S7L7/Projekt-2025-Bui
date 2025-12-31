//
// Created by bui on 29.11.2025.
//
#include <iostream>
#include <string>
#include "DataFetch.h"
#include "Attendance_log.h"

using namespace std;

int main() {
        sqlite3* db = openDatabase("../attendance.db");
        if (!db) return 1;
    while (true) {
        string rfid;
        cout << "Zadejte ID: ";
        cin >> rfid;

        if (rfid == "EXIT") {
            break;
        }

        int employeeId = findemployeeId(db, rfid);

        if (employeeId == -1) {
            cout << "Zamestnanec nenalezen" << endl;
            continue;
        }

        cout << "Zamestnanec nalezen s prirazenym ID: " << employeeId << endl;

        string type = nextEventType(db,employeeId);
        cout << "Entry/Exit: " << type << endl;

        string employeeName = getEmployeeName(db, employeeId);

        if (insertAttendance(db, employeeId, type)) {
            cout << "Dochazka zapsana" << endl;
            writeAttendanceToFile(
            to_string(employeeId)+ " | " + employeeName,
                type);
        } else {
            cout << "Error: Nezapsano" << endl;
        }
    }
        sqlite3_close(db);
        return 0;
}