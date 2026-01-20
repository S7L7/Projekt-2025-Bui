CREATE TABLE attendance (
id INTEGER PRIMARY KEY AUTOINCREMENT,
employee_id INTEGER NOT NULL,
timestamp TEXT NOT NULL DEFAULT (datetime('now')),
type TEXT NOT NULL,
FOREIGN KEY (employee_id) REFERENCES employees(id)


// Přidávání zaměstnanců/ odebrání zaměstnanců
INSERT INTO employees (name, rfid_uid) VALUES ('Jmeno', 'RFID');
SELECT * FROM employees;
UPDATE employees SET name='Nove' WHERE id=1;
DELETE FROM employees WHERE id=1;