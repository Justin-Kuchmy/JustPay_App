
CREATE TABLE IF NOT EXISTS budget_periods (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    label       TEXT    NOT NULL,         
    year INTEGER   NOT NULL,      
    half        INTEGER NOT NULL CHECK (half IN (1, 2)),
    start_date  TEXT    NOT NULL,          
    end_date    TEXT    NOT NULL
);
