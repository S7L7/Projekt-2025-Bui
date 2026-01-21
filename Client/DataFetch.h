//
// Created by buijo on 30.12.2025.
//

#ifndef DOCHAZKOVY_SYSTEM_DATAFETCH_H
#define DOCHAZKOVY_SYSTEM_DATAFETCH_H

#include <string>
#include <ctime>
#include <vector>
#include "../Client/amalgamations/sqlite3.h"
#include "Employee.h"

sqlite3* openDatabase(const std::string &filename);
int findemployeeId(sqlite3* db, const std::string& rfid);
bool insertAttendance(sqlite3* db,int employeeId, const std::string &type);
std::string nextEventType(sqlite3* db, int employeeId);
std::string getEmployeeName(sqlite3* db, int employeeId);
Employee getEmployeeByRfid(sqlite3* db, const std::string& rfid);
bool updateEmployeeStatus(sqlite3* db, int employeeId, int newStatus);
bool addEmployee(sqlite3* db, const std::string& name, const std::string& rfid);
bool deactivateEmployee(sqlite3* db, int employeeId);
bool activateEmployee(sqlite3* db, int employeeId);

struct AttendanceRecord {
std::string type;
std::string timestamp;
};
std::vector<AttendanceRecord>
getAttendanceHistory(sqlite3* db, int employeeId);
long long calculateWorkedSeconds(
    const std::vector<AttendanceRecord>& records);

#endif //DOCHAZKOVY_SYSTEM_DATAFETCH2_H