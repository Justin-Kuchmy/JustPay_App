CREATE TABLE IF NOT EXISTS employees (
tableId INTEGER PRIMARY KEY AUTOINCREMENT,
employeeId TEXT UNIQUE,
fullName TEXT NOT NULL,
department INTEGER NOT NULL,
position TEXT,
jobLevel INTEGER NOT NULL,
status INTEGER NOT NULL,
dateHired TEXT NOT NULL,
dateSeparation TEXT,
sssNumber TEXT,
philHealthNumber TEXT,
hdmfNumber TEXT,
tin TEXT,
bankAccountNumber TEXT,
clockInTimeStr TEXT,
clockOutTimeStr TEXT,
monthlyBasicSalary REAL,
monthlyAllowances REAL,
personalEmail TEXT,
personalMobileNumber TEXT,
isActive INTEGER,
contactId INTEGER,
dependentId INTEGER,
FOREIGN KEY (contactId) REFERENCES emergency_contacts(contactId) ON DELETE SET NULL ON UPDATE CASCADE,
FOREIGN KEY (dependentId) REFERENCES dependents(dependentId) ON DELETE SET NULL ON UPDATE CASCADE

);


CREATE TRIGGER IF NOT EXISTS set_employeeId
AFTER INSERT ON employees
FOR EACH ROW
BEGIN
UPDATE employees
SET employeeId = printf('%02d', NEW.department) || '-' || printf('%04d', NEW.tableId)
WHERE tableId = NEW.tableId;
END;
