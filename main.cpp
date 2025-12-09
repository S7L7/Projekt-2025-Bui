//
// Created by buijo on 29.11.2025.
//
#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;

sqlite3* openDatabase(const string &filename) {
    sqlite3* db=nullptr;
    if(sqlite3_open(filename.c_str(), &db) != SQLITE_OK){
        cerr << "Chyba pri otevirani databaze: " << sqlite3_errmsg(db) << endl;
        return nullptr;
    }
    return db;
};