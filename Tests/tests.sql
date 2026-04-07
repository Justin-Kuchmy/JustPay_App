
DROP TABLE IF EXISTS emergency_contacts;
DROP TABLE IF EXISTS dependents;
DROP TABLE IF EXISTS loan_ledgers;
DROP TABLE IF EXISTS attendance_log;
DROP TABLE IF EXISTS payroll_records;
DROP TABLE IF EXISTS employees;
DROP TABLE IF EXISTS employee_leave_balances;
DROP TABLE IF EXISTS test_item;
DROP TABLE IF EXISTS department_budgets;
DROP TABLE IF EXISTS budget_periods;



CREATE TABLE  IF NOT EXISTS payroll_config (
    id integer PRIMARY KEY AUTOINCREMENT,
    sss_schedule INTEGER NOT NULL DEFAULT 1,
    philhealth_schedule INTEGER NOT NULL DEFAULT 2,
    hdmf_schedule INTEGER NOT NULL DEFAULT 2,

    CHECK (sss_schedule        IN (1, 2, 3)),
    CHECK (philhealth_schedule IN (1, 2, 3)),
    CHECK (hdmf_schedule       IN (1, 2, 3))
);


INSERT INTO payroll_config (sss_schedule, philhealth_schedule, hdmf_schedule) VALUES
(1, 2, 2);

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
    employee_Contrib REAL DEFAULT 0.0,
    employer_Contrib REAL DEFAULT 0.0,
    total_Contrib REAL DEFAULT 0.0,
    remittance_type INTEGER DEFAULT 0, 
    submission_status INTEGER DEFAULT 0, 
    withholding_tax REAL DEFAULT 0.0,
    withholding_tax_submission_status INTEGER DEFAULT 0,
    last_submitted_date TEXT,
    submitted_by_user_id INTEGER DEFAULT 0,
    date_created TEXT DEFAULT CURRENT_TIMESTAMP,
    date_modified TEXT DEFAULT CURRENT_TIMESTAMP,

    FOREIGN KEY (payroll_calculation_results_id) REFERENCES payroll_calculation_results(id)
);


INSERT INTO "emergency_contacts" (name, relation, address, contactNo) VALUES 
('Juan Santos','Brother','Quezon City','09171234567'),
('Carlos Mendoza', 'Husband', '123 Orchid St, Quezon City', '09171234567'),
('Lina Cruz', 'Mother', '45 Pine Ave, Makati City', '09181234567'),
('Robert Reyes', 'Father', '89 Palm Blvd, Taguig', '09193456789'),
('Patricia Tan', 'Wife', '22 Acacia Dr, Pasig', '09204567890'),
('Andrea Lim', 'Sister', '77 Maple Rd, Mandaluyong', '09175554433'),
('Fernando Torres', 'Brother', '66 Laurel St, Paranaque', '09206667788'),
('Irene Navarro', 'Aunt', '21 Santol St, Marikina', '09186668899'),
('Diego Ramos', 'Brother', '34 Cherry Ln, San Juan', '09217773344'),
('Nathan Uy', 'Father', '56 Molave Ave, Las Piñas', '09174445566'),
('Cecilia Bautista', 'Mother', '88 Mabini St, Caloocan', '09218882211');

INSERT INTO "dependents" (name, relation, birthday) VALUES 
('Juan Santos','Brother','1991-05-27'),
('Lara Mendoza', 'Daughter', '2016-03-14'),
('Ethan Cruz', 'Son', '2018-09-21'),
('Sofia Reyes', 'Spouse', '1992-07-10'),
('Jacob Tan', 'Son', '2013-11-05'),
('Angela Lim', 'Mother', '1964-01-28'),
('Miguel Torres', 'Father', '1960-06-19'),
('Chloe Navarro', 'Daughter', '2017-12-02'),
('Lucas Ramos', 'Son', '2015-05-09'),
('Bianca Uy', 'Spouse', '1990-10-15'),
('Daniel Bautista', 'Brother', '1995-04-04');



INSERT INTO "employees" (fullName, department, position, jobLevel, status, dateHired, dateSeparation,sssNumber, philHealthNumber, hdmfNumber,tin, bankAccountNumber,clockInTimeStr,clockOutTimeStr,monthlyBasicSalary, monthlyAllowances, personalEmail, personalMobileNumber, isActive, contactId, dependentId)
VALUES
('Alice Santos',0,'HR Manager',2,0,'2022-01-15',NULL,'12-3456789-0','987654321','123456789','123-456-789','001234567890','08:30','17:00',45000,5000,'alice@example.com','09170010001',1,1,1),
('Bob Reyes',1,'Finance Associate',0,1,'2023-03-01',NULL,'23-9876543-1','123456789','987654321','234-567-890','001234567891','08:30','18:30',28000,2000,'bob@example.com','09170010002',1,2,2),
('Mary Mabulay',1,'Senior Accountant',1,0,'2016-01-15',NULL,'11-3159781-0','387254961','148563279','345-678-901','001234567892','08:30','17:00',80000,5000,'mary@example.com','09170010003',1,3,3),
('Carlos Dela Cruz',2,'Software Engineer',1,0,'2021-05-03',NULL,'14-1234567-9','111222333','555666777','456-789-012','001234567893','08:30','17:00',55000,3000,'carlos@company.com','09170010004',1,4,4),
('Janine Uy',2,'UI/UX Designer',0,1,'2022-11-10',NULL,'17-7654321-0','444555666','333222111','567-890-123','001234567894','08:30','17:00',42000,2500,'janine@company.com','09170010005',1,5,5),
('Patrick Gomez',3,'Marketing Specialist',0,0,'2020-02-14',NULL,'22-1112223-4','777888999','555444333','678-901-234','001234567895','08:30','17:00',38000,1500,'patrick@company.com','09170010006',1,6,6),
('Lea Navarro',3,'Marketing Manager',2,0,'2018-08-01',NULL,'25-9998887-6','222111000','999888777','789-012-345','001234567896','08:30','17:00',65000,5000,'lea@company.com','09170010007',1,7,7),
('Jasmine Co',4,'Admin Assistant',0,1,'2021-12-20',NULL,'30-6543210-9','555444333','222333444','890-123-456','001234567897','08:30','17:00',25000,1200,'jasmine@company.com','09170010008',1,8,8),
('Miguel Tan',4,'Operations Supervisor',1,0,'2019-03-25',NULL,'31-1230987-4','666777888','111222333','901-234-567','001234567898','08:30','17:00',47000,3000,'miguel@company.com','09170010009',1,9,9),
('Katrina Ramos',5,'IT Support',0,1,'2023-06-05',NULL,'32-3216549-8','555666777','888999000','012-345-678','001234567899','08:30','17:00',30000,1500,'katrina@company.com','09170010010',1,10,10);



INSERT INTO loan_ledgers (employeeId, loanType, principalAmount, loanDate, NumOfAmortizations, deductionsPerPayroll, deductionFirstHalf, deductionSecondHalf, status) VALUES
('00-0001', 1, 50000.00, '2024-06-01', 24, 1041.67, 1, 1, 1),
('01-0003', 2, 20000.00, '2024-09-01', 6, 1666.67, 0, 1, 1),
('02-0004', 1, 30000.00, '2024-11-01', 12, 1250.00, 1, 1, 1),
('04-0009', 2, 15000.00, '2024-10-01', 3, 2500.00, 0, 1, 1);

INSERT INTO attendance_log (employeeId, log_date, late_min, undertime_min, overtime_min, absent, notes, overtime_json) VALUES
('00-0001','2025-01-15',0,0,90,0,NULL,'{"regular":90,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('00-0001','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('01-0002','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('01-0002','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('01-0003','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('01-0003','2025-01-31',0,0,60,0,NULL,'{"regular":60,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('02-0004','2025-01-15',0,0,120,0,NULL,'{"regular":120,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('02-0004','2025-01-31',0,0,180,0,NULL,'{"regular":180,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('02-0005','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('02-0005','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('03-0006','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('03-0006','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('03-0007','2025-01-15',0,0,150,0,NULL,'{"regular":150,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('03-0007','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('04-0008','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('04-0008','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('04-0009','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('04-0009','2025-01-31',0,0,90,0,NULL,'{"regular":90,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('05-0010','2025-01-15',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}'),
('05-0010','2025-01-31',0,0,0,0,NULL,'{"regular":0,"rest_day":0,"rest_day_plus":0,"legal_holiday":0,"legal_holiday_plus":0,"special_holiday":0,"special_holiday_plus":0,"rest_plus_legal":0,"rest_plus_special":0,"night_shift_diff":0}');

INSERT INTO payroll_records (employee_id,full_name,department,pay_period_text,pay_period_half,basic_salary,allowances,overtime_pay,adjustments,gross_income,sss_premium_ee,philhealth_premium_ee,hdmf_premium_ee,loan_deductions,withholding_tax,total_deductions,net_pay,sss_premium_er,philhealth_premium_er,hdmf_premium_er) VALUES
('00-0001','Alice Santos','HR','January 2025',1,22500.00,2500.00,404.14,0.00,25404.14,1125.00,0.00,0.00,1041.67,0.00,2166.67,23237.47,1788.75,0.00,0.00),
('00-0001','Alice Santos','HR','January 2025',2,22500.00,2500.00,0.00,0.00,25000.00,0.00,562.50,200.00,1041.67,1698.07,3502.24,21497.76,0.00,562.50,200.00),
('01-0002','Bob Reyes','Finance','January 2025',1,14000.00,1000.00,0.00,0.00,15000.00,700.00,0.00,0.00,0.00,0.00,700.00,14300.00,1113.00,0.00,0.00),
('01-0002','Bob Reyes','Finance','January 2025',2,14000.00,1000.00,0.00,0.00,15000.00,0.00,350.00,200.00,0.00,454.95,1004.95,13995.05,0.00,350.00,200.00),
('01-0003','Mary Mabulay','Finance','January 2025',1,40000.00,2500.00,0.00,0.00,42500.00,1750.00,0.00,0.00,0.00,0.00,1750.00,40750.00,2782.50,0.00,0.00),
('01-0003','Mary Mabulay','Finance','January 2025',2,40000.00,2500.00,479.30,0.00,42979.30,0.00,1000.00,200.00,1666.67,5757.28,8623.95,34355.35,0.00,1000.00,200.00),
('02-0004','Carlos Dela Cruz','IT','January 2025',1,27500.00,1500.00,658.95,0.00,29658.95,1375.00,0.00,0.00,1250.00,0.00,2625.00,27033.95,2186.25,0.00,0.00),
('02-0004','Carlos Dela Cruz','IT','January 2025',2,27500.00,1500.00,988.43,0.00,29988.43,0.00,687.50,200.00,1250.00,3124.49,5261.99,24726.44,0.00,687.50,200.00),
('02-0005','Janine Uy','IT','January 2025',1,21000.00,1250.00,0.00,0.00,22250.00,1050.00,0.00,0.00,0.00,0.00,1050.00,21200.00,1669.50,0.00,0.00),
('02-0005','Janine Uy','IT','January 2025',2,21000.00,1250.00,0.00,0.00,22250.00,0.00,525.00,200.00,0.00,1659.10,2384.10,19865.90,0.00,525.00,200.00),
('03-0006','Patrick Gomez','Operations','January 2025',1,19000.00,750.00,0.00,0.00,19750.00,950.00,0.00,0.00,0.00,0.00,950.00,18800.00,1510.50,0.00,0.00),
('03-0006','Patrick Gomez','Operations','January 2025',2,19000.00,750.00,0.00,0.00,19750.00,0.00,475.00,200.00,0.00,1269.10,1944.10,17805.90,0.00,475.00,200.00),
('03-0007','Lea Navarro','Operations','January 2025',1,32500.00,2500.00,973.31,0.00,35973.31,1625.00,0.00,0.00,0.00,0.00,1625.00,34348.31,2583.75,0.00,0.00),
('03-0007','Lea Navarro','Operations','January 2025',2,32500.00,2500.00,0.00,0.00,35000.00,0.00,812.50,200.00,0.00,3901.60,4914.10,30085.90,0.00,812.50,200.00),
('04-0008','Jasmine Co','Sales','January 2025',1,12500.00,600.00,0.00,0.00,13100.00,625.00,0.00,0.00,0.00,0.00,625.00,12475.00,993.75,0.00,0.00),
('04-0008','Jasmine Co','Sales','January 2025',2,12500.00,600.00,0.00,0.00,13100.00,0.00,312.50,200.00,0.00,235.57,748.07,12351.93,0.00,312.50,200.00),
('04-0009','Miguel Tan','Sales','January 2025',1,23500.00,1500.00,0.00,0.00,25000.00,1175.00,0.00,0.00,0.00,0.00,1175.00,23825.00,1868.25,0.00,0.00),
('04-0009','Miguel Tan','Sales','January 2025',2,23500.00,1500.00,422.18,0.00,25422.18,0.00,587.50,200.00,2500.00,2231.04,5518.54,19903.64,0.00,587.50,200.00),
('05-0010','Katrina Ramos','Marketing','January 2025',1,15000.00,750.00,0.00,0.00,15750.00,750.00,0.00,0.00,0.00,0.00,750.00,15000.00,1192.50,0.00,0.00),
('05-0010','Katrina Ramos','Marketing','January 2025',2,15000.00,750.00,0.00,0.00,15750.00,0.00,375.00,200.00,0.00,601.20,1176.20,14573.80,0.00,375.00,200.00);

CREATE TABLE IF NOT EXISTS base_repo_test_table (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL
);

INSERT INTO base_repo_test_table (name) VALUES ('Alice');
INSERT INTO base_repo_test_table (name) VALUES ('Bob');
INSERT INTO base_repo_test_table (name) VALUES ('Charlie');


CREATE TABLE employee_leave_balances (
    employee_id TEXT,
    year INTEGER,
    total_leave_earned REAL,
    leave_used REAL,
    PRIMARY KEY (employee_id, year)
);


CREATE TABLE IF NOT EXISTS budget_periods (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    label       TEXT    NOT NULL,         
    year INTEGER   NOT NULL,      
    half        INTEGER NOT NULL CHECK (half IN (1, 2)),
    start_date  TEXT    NOT NULL,          
    end_date    TEXT    NOT NULL
);


INSERT INTO budget_periods (label, year, half, start_date, end_date) VALUES
('January 2025 - 1st Half', 2025, 1, '2025-01-01', '2025-01-15'),
('January 2025 - 2nd Half', 2025, 2, '2025-01-16', '2025-01-31');




CREATE TABLE department_budgets (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    department INTEGER    NOT NULL,   
    period_id INTEGER NOT NULL REFERENCES budget_periods(id),
    allocated_amount REAL NOT NULL DEFAULT 0.0,
    notes TEXT,
    UNIQUE (department, period_id)     
);

INSERT INTO department_budgets (department, period_id, allocated_amount, notes) VALUES
( 0, 1, 27672.70, 'January 2025 1st half - HR'),
( 1, 1, 63730.70, 'January 2025 1st half - Finance'),
( 2, 1, 57036.10, 'January 2025 1st half - IT');


PRAGMA foreign_keys = OFF; 

