#include <iostream>
#include "DataFetch.h"

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

string nextEventType(sqlite3* db, int employeeId){
const char* sql =
    "SELECT type FROM attendance "
    "WHERE employee_id = ? "
    "ORDER BY timestamp DESC "
    "LIMIT 1;";
sqlite3_stmt* stmt = nullptr;

if (sqlite3_prepare_v2(db,sql, -1 , &stmt, nullptr) != SQLITE_OK) {
    cerr << "Chyba pri priprave nacteni posledniho zaznamu: " <<  sqlite3_errmsg(db) << endl;
    return "entry";
}

sqlite3_bind_int (stmt, 1 , employeeId);
    string result = "entry";

    int rc=sqlite3_step(stmt);

    if (rc == SQLITE_ROW) {
        string last = reinterpret_cast<const char*>(sqlite3_column_text(stmt,0));
        if (last == "entry")
            result = "exit";
        else result = "entry";
    }

    sqlite3_finalize(stmt);
    return result;
}