BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS journal_entries (
    entryId INTEGER PRIMARY KEY AUTOINCREMENT,
    accountType TEXT NOT NULL,     
    accountName TEXT NOT NULL,
    debit REAL NOT NULL,  
    credit REAL NOT NULL,
    pay_period_date   VARCHAR(50)  NOT NULL,
    pay_period_half  INTEGER      NOT NULL CHECK (pay_period_half IN (1, 2))
);




COMMIT;