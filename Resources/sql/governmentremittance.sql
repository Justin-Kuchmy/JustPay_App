BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS government_remittances (
	id	INTEGER,
	payroll_calculation_results_id	INTEGER NOT NULL,
	employee_id	TEXT NOT NULL,
	full_name	TEXT NOT NULL,
	employee_department	INTEGER,
	pay_period_date	TEXT NOT NULL,
	pay_period_half	INTEGER NOT NULL,
	employee_Contrib	REAL DEFAULT 0.0,
	employer_Contrib	REAL DEFAULT 0.0,
	total_Contrib	REAL DEFAULT 0.0,
	remittance_type	INTEGER DEFAULT 0,
	submission_status	INTEGER DEFAULT 0,
	withholding_tax	REAL DEFAULT 0.0,
	withholding_tax_submission_status	INTEGER DEFAULT 0,
	last_submitted_date	TEXT,
	submitted_by_user_id	INTEGER DEFAULT 0,
	date_created	TEXT DEFAULT CURRENT_TIMESTAMP,
	date_modified	TEXT DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY(id AUTOINCREMENT),
	FOREIGN KEY(payroll_calculation_results_id) REFERENCES payroll_calculation_results(id)
);

CREATE INDEX IF NOT EXISTS idx_gov_rem_employee_id ON government_remittances (
	employee_id
);
CREATE INDEX IF NOT EXISTS idx_gov_rem_pay_period ON government_remittances (
	pay_period_text,
	pay_period_half
);
COMMIT;
