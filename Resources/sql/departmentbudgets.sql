CREATE TABLE IF NOT EXISTS department_budgets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    department INTEGER    NOT NULL,   
    period_id INTEGER NOT NULL REFERENCES budget_periods(id),
    allocated_amount REAL NOT NULL DEFAULT 0.0,
    notes TEXT,
    UNIQUE (department, period_id)     
);

