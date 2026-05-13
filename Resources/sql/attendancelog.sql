BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS attendance_log (
    logId INTEGER PRIMARY KEY AUTOINCREMENT,
    employeeId TEXT,
    log_date TEXT,
    late_min INTEGER,
    undertime_min INTEGER,
    overtime_min INTEGER,
    absent INTEGER default 0,
    notes TEXT,
    overtime_json TEXT,
    FOREIGN KEY (employeeId) REFERENCES employees(employeeId) ON DELETE SET NULL ON UPDATE CASCADE
);
COMMIT;
