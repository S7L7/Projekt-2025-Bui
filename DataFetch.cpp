#include <iostream>
#include "DataFetch.h"
#include "Employee.h"
#include "adminmenu.h"

using namespace std;

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
    const char* sql = "UPDATE employees SET active = ? WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Chyba pri priprave aktualizace dat: " << sqlite3_errmsg(db) << endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, active);
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