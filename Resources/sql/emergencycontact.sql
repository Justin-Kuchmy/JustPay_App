BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS emergency_contacts (
    contactId INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    relation TEXT NOT NULL,
    address TEXT,
    contactNo TEXTNOT NULL
    
);

COMMIT;