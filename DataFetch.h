//
// Created by buijo on 30.12.2025.
//

#ifndef DOCHAZKOVY_SYSTEM_DATAFETCH_H
#define DOCHAZKOVY_SYSTEM_DATAFETCH_H

#include <string>
#include "sqlite3.h"


sqlite3* openDatabase(const std::string &filename);
int findemployeeId(sqlite3* db, const std::string& rfid);
bool insertAttendance(sqlite3* db,int employeeId, const std::string &type);
std::string nextEventType(sqlite3* db, int employeeId);
std::string getEmployeeName(sqlite3* db, int employeeId);
#endif //DOCHAZKOVY_SYSTEM_DATAFETCH2_H