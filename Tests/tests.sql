
DROP TABLE IF EXISTS emergency_contacts;
DROP TABLE IF EXISTS dependents;
DROP TABLE IF EXISTS loan_ledgers;
DROP TABLE IF EXISTS attendance_log;
DROP TABLE IF EXISTS payroll_records;
DROP TABLE IF EXISTS employees;
DROP TABLE IF EXISTS test_item;



CREATE TABLE  IF NOT EXISTS payroll_config (
    id integer PRIMARY KEY AUTOINCREMENT,
    sss_schedule INTEGER NOT NULL DEFAULT 2,
    philhealth_schedule INTEGER NOT NULL DEFAULT 2,
    hdmf_schedule INTEGER NOT NULL DEFAULT 2,

    CHECK (sss_schedule        IN (1, 2, 3)),
    CHECK (philhealth_schedule IN (1, 2, 3)),
    CHECK (hdmf_schedule       IN (1, 2, 3))
);

CREATE TABLE IF NOT EXISTS emergency_contacts (
    contactId INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    relation TEXT NOT NULL,
    address TEXT,
    contactNo TEXT NOT NULL
    
);

CREATE TABLE IF NOT EXISTS dependents (
    dependentId INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    relation TEXT NOT NULL,
    birthday TEXT NOT NULL

);


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

CREATE TABLE IF NOT EXISTS loan_ledgers (
loanLedgerId	INTEGER PRIMARY KEY AUTOINCREMENT,
employeeId TEXT NOT NULL,
loanType	INTEGER NOT NULL,
principalAmount	REAL NOT NULL,
loanDate	TEXT NOT NULL,
NumOfAmortizations INTEGER NOT NULL,
deductionsPerPayroll	TEXT NOT NULL,
deductionFirstHalf  INTEGER NOT NULL DEFAULT 0,
deductionSecondHalf INTEGER NOT NULL DEFAULT 0, 
status TEXT NOT NULL DEFAULT 'Active',
FOREIGN KEY("employeeId") REFERENCES "employees"("employeeId")
);

CREATE TRIGGER set_employeeId
AFTER INSERT ON employees
FOR EACH ROW
BEGIN
    UPDATE employees
    SET employeeId = printf('%02d', NEW.department) || '-' || printf('%04d', NEW.tableId)
    WHERE tableId = NEW.tableId;
END;

CREATE TABLE IF NOT EXISTS attendance_log (
    logId INTEGER PRIMARY KEY AUTOINCREMENT,
    employeeId TEXT,
    log_date TEXT,
    late_min INTEGER,
    undertime_min INTEGER,
    overtime_min INTEGER,
    absent INTEGER default 0,
    notes TEXT,
    overtime_json TEXT
);


CREATE TABLE IF NOT EXISTS payroll_records (
    id INTEGER PRIMARY KEY AUTOINCREMENT, 

    employee_id        VARCHAR(50)  NOT NULL,
    full_name          VARCHAR(100) NOT NULL,
    department         VARCHAR(100) NOT NULL,

    pay_period_text   VARCHAR(50)  NOT NULL, 
    pay_period_half  INTEGER NOT NULL CHECK (pay_period_half IN (1, 2)),

    basic_salary       DECIMAL(12, 2) NOT NULL,
    allowances         DECIMAL(12, 2) NOT NULL,
    overtime_pay       DECIMAL(12, 2) NOT NULL,
    adjustments        DECIMAL(12, 2) NOT NULL,

    gross_income       DECIMAL(12, 2) NOT NULL,

    sss_premium_ee        DECIMAL(12, 2) NOT NULL,
    philhealth_premium_ee DECIMAL(12, 2) NOT NULL,
    hdmf_premium_ee       DECIMAL(12, 2) NOT NULL,
    loan_deductions    DECIMAL(12, 2) NOT NULL,
    withholding_tax    DECIMAL(12, 2) NOT NULL,

    total_deductions   DECIMAL(12, 2) NOT NULL,
    net_pay            DECIMAL(12, 2) NOT NULL,

    created_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    sss_premium_er        DECIMAL(12, 2) NOT NULL,
    philhealth_premium_er DECIMAL(12, 2) NOT NULL,
    hdmf_premium_er       DECIMAL(12, 2) NOT NULL,
    UNIQUE (employee_id, pay_period_text, pay_period_half)
);

CREATE TABLE IF NOT EXISTS government_remittances (

    id INTEGER PRIMARY KEY AUTOINCREMENT,
    

    payroll_calculation_results_id INTEGER NOT NULL,
    employee_id TEXT NOT NULL,
    full_name TEXT NOT NULL,
    employee_department TEXT,
    pay_period_text TEXT NOT NULL,
    pay_period_half INTEGER NOT NULL,
    

    sss_premium_ee REAL DEFAULT 0.0,
    sss_premium_er REAL DEFAULT 0.0,
    sss_premium_total REAL DEFAULT 0.0,
    sss_submission_status INTEGER DEFAULT 0,  -- 0=PENDING, 1=SUBMITTED, 2=CONFIRMED, 3=REJECTED
    

    phic_premium_ee REAL DEFAULT 0.0,
    phic_premium_er REAL DEFAULT 0.0,
    phic_premium_total REAL DEFAULT 0.0,
    phic_submission_status INTEGER DEFAULT 0,

    hdmf_premium_ee REAL DEFAULT 0.0,
    hdmf_premium_er REAL DEFAULT 0.0,
    hdmf_premium_total REAL DEFAULT 0.0,
    hdmf_submission_status INTEGER DEFAULT 0,
    
    withholding_tax REAL DEFAULT 0.0,
    withholding_tax_submission_status INTEGER DEFAULT 0,
    
    last_submitted_date TEXT,
    submitted_by_user_id INTEGER DEFAULT 0,
    date_created TEXT DEFAULT CURRENT_TIMESTAMP,
    date_modified TEXT DEFAULT CURRENT_TIMESTAMP,
    
    FOREIGN KEY (payroll_calculation_results_id) REFERENCES payroll_calculation_results(id)
);

-- Create indexes for common queries
CREATE INDEX IF NOT EXISTS idx_gov_rem_pay_period 
    ON government_remittances(pay_period_text, pay_period_half);

CREATE INDEX IF NOT EXISTS idx_gov_rem_employee_id 
    ON government_remittances(employee_id);

CREATE INDEX IF NOT EXISTS idx_gov_rem_submission_status 
    ON government_remittances(sss_submission_status, phic_submission_status, hdmf_submission_status, withholding_tax_submission_status);

CREATE INDEX IF NOT EXISTS idx_gov_rem_payroll_fk 
    ON government_remittances(payroll_calculation_results_id);
    
INSERT OR REPLACE INTO payroll_config (sss_schedule, philhealth_schedule, hdmf_schedule)
VALUES (2, 2, 2);


INSERT INTO "emergency_contacts" (name, relation, address, contactNo) VALUES ('Juan Santos','Brother','Quezon City','09171234567');

INSERT INTO "dependents" (name, relation, birthday) VALUES ('Juan Santos','Brother','1991-05-27');



INSERT INTO employees (
    fullName, department, position, jobLevel, status,
    dateHired, dateSeparation, sssNumber, philHealthNumber, hdmfNumber,
    tin, bankAccountNumber, clockInTimeStr, clockOutTimeStr, monthlyBasicSalary, monthlyAllowances,
    personalEmail, personalMobileNumber, isActive, contactId, dependentId
)
VALUES (
    'Alice Santos', 0, 'HR Manager', 2, 0,
    '2022-01-15', NULL, '12-3456789-0', '987654321', '123456789',
    '123-456-789', '001234567890', '7:30','18:30',45000, 5000,
    'alice@example.com', '09170010001', 1,
    (SELECT contactId FROM emergency_contacts ORDER BY contactId DESC LIMIT 1),
    (SELECT dependentId FROM dependents ORDER BY dependentId DESC LIMIT 1)
);



INSERT INTO loan_ledgers ("employeeId","loanType","principalAmount","loanDate","NumOfAmortizations","deductionsPerPayroll","deductionFirstHalf","deductionSecondHalf","status") VALUES 
('00-0001',1,50000.00,'2025-11-11',10,'5000.00',1,1,'Active'),
('00-0001',1,25000.00,'2025-11-11',5,'5000.00',1,1,'Active');



INSERT INTO attendance_log ("employeeId", "log_date", "late_min", "undertime_min", "overtime_min", "absent", "notes", "overtime_json") VALUES
('00-0001','2025-11-22',7,0,45,0, 'Test attendance log entry', "{'regular':45,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':45}"),
('00-0001','2025-11-23',10,0,23,0, 'Test attendance log entry', "{'regular':23,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':23}");

INSERT INTO payroll_records (employee_id, full_name, department, pay_period_text, pay_period_half, basic_salary, allowances, overtime_pay, adjustments, gross_income, sss_premium_ee, philhealth_premium_ee, hdmf_premium_ee, loan_deductions, withholding_tax, total_deductions, net_pay, sss_premium_er, philhealth_premium_er, hdmf_premium_er) 
VALUES 
('00-0001','Alice Santos','HR','January 2025',1,22500.00,2500.00,157.00,0.00,25157.00,1125.00,0.00,0.00,2083.33,1907.85,5116.18,20040.82,0.0,0.0,0.0),
('01-0002','Bob Reyes','Finance','January 2025',1,14000.00,1000.00,198.00,0.00,15198.00,700.00,0.00,0.00,5000.00,442.34,6142.34,9055.66,0.0,0.0,0.0),
('01-0003','Mary Mabulay','Finance','January 2025',1,40000.00,2500.00,239.00,0.00,42739.00,1750.00,0.00,0.00,2500.00,5611.95,9861.95,32877.05,0.0,0.0,0.0),
('02-0004','Carlos Dela Cruz','IT','January 2025',1,27500.00,1500.00,280.00,0.00,29280.00,1375.00,0.00,0.00,0.00,2914.00,4289.00,24991.00,0.0,0.0,0.0),
('02-0005','Janine Uy','IT','January 2025',1,21000.00,1250.00,321.00,0.00,22571.00,1050.00,0.00,0.00,0.00,1596.05,2646.05,19924.95,0.0,0.0,0.0),
('03-0006','Patrick Gomez','Operations','January 2025',1,19000.00,750.00,362.00,0.00,20112.00,950.00,0.00,0.00,0.00,1218.10,2168.10,17943.90,0.0,0.0,0.0),
('03-0007','Lea Navarro','Operations','January 2025',1,32500.00,2500.00,403.00,0.00,35403.00,1625.00,0.00,0.00,0.00,3820.15,5445.15,29957.85,0.0,0.0,0.0),
('04-0008','Jasmine Co','Sales','January 2025',1,12500.00,600.00,444.00,0.00,13544.00,625.00,0.00,0.00,0.00,252.20,877.20,12666.80,0.0,0.0,0.0),
('04-0009','Miguel Tan','Sales','January 2025',1,23500.00,1500.00,485.00,0.00,25485.00,1175.00,0.00,0.00,0.00,2093.35,3268.35,22216.65,0.0,0.0,0.0),
('05-0010','Katrina Ramos','Marketing','January 2025',1,15000.00,750.00,526.00,0.00,16276.00,750.00,0.00,0.00,0.00,601.30,1351.30,14924.70,0.0,0.0,0.0),
('00-0001','Alice Santos','HR','January 2025',2,22500.00,2500.00,314.00,0.00,25314.00,1125.00,0.00,0.00,2083.33,1915.70,5124.03,20189.97,0.0,0.0,0.0),
('01-0002','Bob Reyes','Finance','January 2025',2,14000.00,1000.00,355.00,0.00,15355.00,700.00,0.00,0.00,5000.00,450.19,6150.19,9204.81,0.0,0.0,0.0),
('01-0003','Mary Mabulay','Finance','January 2025',2,40000.00,2500.00,396.00,0.00,42896.00,1750.00,0.00,0.00,2500.00,5619.80,9869.80,33026.20,0.0,0.0,0.0),
('02-0004','Carlos Dela Cruz','IT','January 2025',2,27500.00,1500.00,437.00,0.00,29437.00,1375.00,0.00,0.00,0.00,2921.85,4296.85,25140.15,0.0,0.0,0.0),
('02-0005','Janine Uy','IT','January 2025',2,21000.00,1250.00,478.00,0.00,22728.00,1050.00,0.00,0.00,0.00,1603.90,2653.90,20074.10,0.0,0.0,0.0),
('03-0006','Patrick Gomez','Operations','January 2025',2,19000.00,750.00,519.00,0.00,20269.00,950.00,0.00,0.00,0.00,1225.95,2175.95,18093.05,0.0,0.0,0.0),
('03-0007','Lea Navarro','Operations','January 2025',2,32500.00,2500.00,560.00,0.00,35560.00,1625.00,0.00,0.00,0.00,3828.00,5453.00,30107.00,0.0,0.0,0.0),
('04-0008','Jasmine Co','Sales','January 2025',2,12500.00,600.00,1.00,0.00,13101.00,625.00,0.00,0.00,0.00,230.05,855.05,12245.95,0.0,0.0,0.0),
('04-0009','Miguel Tan','Sales','January 2025',2,23500.00,1500.00,42.00,0.00,25042.00,1175.00,0.00,0.00,0.00,2071.20,3246.20,21795.80,0.0,0.0,0.0),
('05-0010','Katrina Ramos','Marketing','January 2025',2,15000.00,750.00,83.00,0.00,15833.00,750.00,0.00,0.00,0.00,579.15,1329.15,14503.85,0.0,0.0,0.0);

CREATE TABLE IF NOT EXISTS base_repo_test_table (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

INSERT INTO base_repo_test_table (name) VALUES ('Alice');
INSERT INTO base_repo_test_table (name) VALUES ('Bob');
INSERT INTO base_repo_test_table (name) VALUES ('Charlie');

PRAGMA foreign_keys = OFF; 