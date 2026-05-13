BEGIN TRANSACTION;
CREATE TABLE  IF NOT EXISTS payroll_config (
    id integer PRIMARY KEY AUTOINCREMENT,
    sss_schedule INTEGER NOT NULL DEFAULT 1,
    philhealth_schedule INTEGER NOT NULL DEFAULT 2,
    hdmf_schedule INTEGER NOT NULL DEFAULT 2,

    CHECK (sss_schedule        IN (1, 2, 3)),
    CHECK (philhealth_schedule IN (1, 2, 3)),
    CHECK (hdmf_schedule       IN (1, 2, 3))
);

CREATE TABLE payroll_records (
id INTEGER PRIMARY KEY AUTOINCREMENT, 

employee_id        VARCHAR(50)  NOT NULL,
full_name          VARCHAR(100) NOT NULL,
department         VARCHAR(100) NOT NULL,

pay_period_date   VARCHAR(50)  NOT NULL,
pay_period_half  INTEGER      NOT NULL CHECK (pay_period_half IN (1, 2)),

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
hdmf_premium_er       DECIMAL(12, 2) NOT NULL
);

COMMIT;