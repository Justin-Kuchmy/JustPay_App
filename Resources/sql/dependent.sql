CREATE TABLE IF NOT EXISTS dependents (
    dependentId INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    relation TEXT NOT NULL,
    birthday TEXT NOT NULL

);
