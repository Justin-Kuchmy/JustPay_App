CREATE TABLE IF NOT EXISTS loan_ledgers (
loanLedgerId	INTEGER PRIMARY KEY AUTOINCREMENT,
employeeId TEXT NOT NULL,
loanType	INTEGER NOT NULL,
principalAmount	REAL NOT NULL,
loanDate	TEXT NOT NULL,
NumOfAmortizations INTEGER NOT NULL,
deductionsPerPayroll	REAL NOT NULL,
deductionFirstHalf  INTEGER NOT NULL DEFAULT 0,
deductionSecondHalf INTEGER NOT NULL DEFAULT 0, 
status INTEGER NOT NULL DEFAULT 0,
FOREIGN KEY(employeeId) REFERENCES employees(employeeId)
);
