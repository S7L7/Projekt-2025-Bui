CREATE TABLE attendance (
id INTEGER PRIMARY KEY AUTOINCREMENT,
employee_id INTEGER NOT NULL,
timestamp TEXT NOT NULL DEFAULT (datetime('now')),
type TEXT NOT NULL,
FOREIGN KEY (employee_id) REFERENCES employees(id)
);