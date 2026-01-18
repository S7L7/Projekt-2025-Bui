#include <iostream>
#include <sstream>
#include <iomanip>
#include "DataFetch.h"
#include "Employee.h"
#include "adminmenu.h"

using namespace std;

time_t parseTimestamp(const string& ts) {
std::tm tm = {};
std::istringstream ss(ts);

ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
tm.tm_sec = 0;

return std::mktime(&tm);
}

sqlite3* openDatabase(const string &filename) {
    sqlite3* db=nullptr;
    if(sqlite3_open(filename.c_str(), &db) != SQLITE_OK){
        cerr << "Chyba pri otevirani databaze: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
    }
int findemployeeId(sqlite3* db, const string& rfid) {
    const char* sql= "SELECT id FROM employees WHERE rfid_uid = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db,sql, -1 , &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error : priprava k vyberu: " << sqlite3_errmsg(db) << endl;
        return -1;
    }
    sqlite3_bind_text(stmt, 1, rfid.c_str(), -1, SQLITE_TRANSIENT);


    int rc = sqlite3_step(stmt);
    int employeeId = -1;

    if (rc == SQLITE_ROW) {
        employeeId = sqlite3_column_int(stmt,0);
    }

    sqlite3_finalize(stmt);
    return employeeId;
}
// Vložení docházky zaměstnance, vrací pouze T/F
bool insertAttendance(sqlite3* db,int employeeId, const string &type) {
    const char* sql = "INSERT INTO attendance (employee_id, type)"
                      " VALUES(?,?);";
    sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Chyba pri priprave vkladani dat: " << sqlite3_errmsg(db) << endl;
            return false;
        }
    sqlite3_bind_int(stmt, 1, employeeId);
    sqlite3_bind_text(stmt, 2, type.c_str(), -1, SQLITE_TRANSIENT);
    int rc=sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Chyba pri vlozeni dat" <<  sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
//získání jména zaměstnance z tabulky
string getEmployeeName(sqlite3* db, int employeeId) {
    const char* sql =
        "SELECT name FROM employees WHERE id = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db,sql,-1, &stmt, nullptr) != SQLITE_OK) {
    return "UNKNOWN";
    }
    sqlite3_bind_int(stmt, 1, employeeId);

    string name = "UNKNOWN";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        name = reinterpret_cast<const char*>(sqlite3_column_text(stmt,0));
    }
    sqlite3_finalize(stmt);
    return name;
}
//získání daného zaměstnance podle jejich RFID  (vložení RFID,vrátí se jméno stav atd.)
Employee getEmployeeByRfid(sqlite3* db, const std::string& rfid) {
    const char* sql = "SELECT id, name, rfid_uid, status FROM employees WHERE rfid_uid = ? LIMIT 1;";
    sqlite3_stmt* stmt = nullptr;

    Employee emp;
    emp.id = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return emp;
    }
    sqlite3_bind_text(stmt, 1, rfid.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        emp.id = sqlite3_column_int(stmt,0);
        emp.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt,1));
        emp.rfid = reinterpret_cast<const char*>(sqlite3_column_text(stmt,2));
        emp.status = sqlite3_column_int(stmt,3);
    }
    sqlite3_finalize(stmt);
    return emp;
}
// Stav zaměstnance uvnitř/venku 1/0
bool updateEmployeeStatus(sqlite3 *db, int employeeId, int newStatus) {
    const char* sql = "UPDATE employees SET status = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Chyba pri priprave aktualizace dat: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, newStatus);
    sqlite3_bind_int(stmt, 2, employeeId);
    int rc=sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Chyba pri vlozeni dat" <<  sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
// přidá se zaměstnanec v admin modu
bool addEmployee(sqlite3 *db, const std::string& name, const std::string &rfid) {
    const char* sql =
        "INSERT INTO employees (name, rfid_uid) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Chyba pri priprave vkladani dat: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, rfid.c_str(), -1, SQLITE_TRANSIENT);
    int rc=sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Chyba pri vlozeni dat" <<  sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
bool deactivateEmployee(sqlite3* db, int employeeId) {
    const char* sql = "UPDATE employees SET active = 0 WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Chyba pri priprave deaktivace zamestnance: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, employeeId);

    int rc=sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Chyba pri deaktivaci" <<  sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
bool activateEmployee(sqlite3* db, int employeeId) {
    const char* sql =
        "UPDATE employees SET active = 1 WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Chyba pri priprave aktivaci zamestnance: "
             << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, employeeId);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        cerr << "Chyba pri aktivaci zamestnance: "
             << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

std::vector<AttendanceRecord>
getAttendanceHistory(sqlite3* db, int employeeId) {
    std::vector<AttendanceRecord> records;

    const char* sql=
        "SELECT type, timestamp "
        "FROM attendance"
        "WHERE employee_id = ?"
        "ORDER BY timestamp;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Chyba pri nacteni historie: " << sqlite3_errmsg(db) << std::endl;
    return records;
    }

    sqlite3_bind_int(stmt, 1, employeeId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        AttendanceRecord rec;
        rec.type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)
        );
        rec.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)
        );
        records.push_back(rec);
    }

    sqlite3_finalize(stmt);
    return records;
}
long long calculateWorkedSeconds(
const vector<AttendanceRecord>& records
){
long long totalseconds = 0;
bool hasEntry = false;
std::time_t entryTime = 0;

for (const auto& rec: records) {
    if(rec.type == "entry") {
    entryTime = parseTimestamp(rec.timestamp);
    hasEntry = true;
    }else if (rec.type == "exit") {
    time_t exitTime = parseTimestamp(rec.timestamp);
    totalseconds += (exitTime - entryTime);
    hasEntry = false;
    }
}
return totalseconds;
}
