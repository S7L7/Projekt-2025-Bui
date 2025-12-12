create table employees (
id INTEGER PRIMARY key AUTOINCREMENT,
name TEXT NOT NULL,
rfid_uid TEXT NOT NULL UNIQUE
);