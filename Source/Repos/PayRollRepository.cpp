#include "Repositories/PayrollRepository.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

PayrollRepository::PayrollRepository(sqlite3 *db) : BaseRepository(db)
{
    LOG_DEBUG("\n PayrollRepository created");
};
bool PayrollRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/payroll.sql");
};

PayrollConfig PayrollRepository::mapPayrollConfig(sqlite3_stmt *stmt)
{
    PayrollConfig config;
    config.sssSchedule = static_cast<DeductionSchedule>(sqlite3_column_int(stmt, 1));
    config.philHealthSchedule = static_cast<DeductionSchedule>(sqlite3_column_int(stmt, 2));
    config.hdmfSchedule = static_cast<DeductionSchedule>(sqlite3_column_int(stmt, 3));
    return config;
}
void PayrollRepository::bindPayroll(sqlite3_stmt *stmt, const PayrollCalculationResults &prRecord)
{
    int idx = 1;
    sqlite3_bind_text(stmt, idx++, prRecord.employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, prRecord.fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, prRecord.employeeDepartment);
    sqlite3_bind_text(stmt, idx++, prRecord.payPeriodDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, prRecord.payPeriodHalf);

    sqlite3_bind_double(stmt, idx++, prRecord.monthlyBasicSalary);
    sqlite3_bind_double(stmt, idx++, prRecord.monthlyAllowances);
    sqlite3_bind_double(stmt, idx++, prRecord.overTimePay);
    sqlite3_bind_double(stmt, idx++, prRecord.adjustments);
    sqlite3_bind_double(stmt, idx++, prRecord.grossIncome);

    sqlite3_bind_double(stmt, idx++, prRecord.sssPremium_EE);
    sqlite3_bind_double(stmt, idx++, prRecord.philHealthPremium_EE);
    sqlite3_bind_double(stmt, idx++, prRecord.hdmfPremium_EE);
    sqlite3_bind_double(stmt, idx++, prRecord.loanDeductionsPerPayroll);

    sqlite3_bind_double(stmt, idx++, prRecord.withHoldingTax);
    sqlite3_bind_double(stmt, idx++, prRecord.totalDeductions);
    sqlite3_bind_double(stmt, idx++, prRecord.netPay);

    sqlite3_bind_text(stmt, idx++, prRecord.dateProcessed.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, idx++, prRecord.sssPremium_ER);
    sqlite3_bind_double(stmt, idx++, prRecord.philHealthPremium_ER);
    sqlite3_bind_double(stmt, idx++, prRecord.hdmfPremium_ER);
}

PayrollCalculationResults PayrollRepository::mapPayroll(sqlite3_stmt *stmt)
{
    PayrollCalculationResults payrollRecord;

    payrollRecord.id = sqlite3_column_int(stmt, 0);
    const unsigned char *employeeId = sqlite3_column_text(stmt, 1);
    payrollRecord.employeeId = employeeId ? reinterpret_cast<const char *>(employeeId) : std::string{};

    const unsigned char *fullName = sqlite3_column_text(stmt, 2);
    payrollRecord.fullName = fullName ? reinterpret_cast<const char *>(fullName) : std::string{};

    payrollRecord.employeeDepartment = sqlite3_column_int(stmt, 3);

    const unsigned char *payPeriodDate = sqlite3_column_text(stmt, 4);
    payrollRecord.payPeriodDate = payPeriodDate ? reinterpret_cast<const char *>(payPeriodDate) : std::string{};

    payrollRecord.payPeriodHalf = sqlite3_column_int(stmt, 5);

    payrollRecord.monthlyBasicSalary = sqlite3_column_double(stmt, 6);
    payrollRecord.monthlyAllowances = sqlite3_column_double(stmt, 7);
    payrollRecord.overTimePay = sqlite3_column_double(stmt, 8);
    payrollRecord.adjustments = sqlite3_column_double(stmt, 9);
    payrollRecord.grossIncome = sqlite3_column_double(stmt, 10);
    payrollRecord.sssPremium_EE = sqlite3_column_double(stmt, 11);
    payrollRecord.philHealthPremium_EE = sqlite3_column_double(stmt, 12);
    payrollRecord.hdmfPremium_EE = sqlite3_column_double(stmt, 13);
    payrollRecord.loanDeductionsPerPayroll = sqlite3_column_double(stmt, 14);
    payrollRecord.withHoldingTax = sqlite3_column_double(stmt, 15);
    payrollRecord.totalDeductions = sqlite3_column_double(stmt, 16);
    payrollRecord.netPay = sqlite3_column_double(stmt, 17);
    const unsigned char *date = sqlite3_column_text(stmt, 18);
    if (date)
    {
        payrollRecord.dateProcessed = Date::fromString(reinterpret_cast<const char *>(date));
    }
    else
    {
        payrollRecord.dateProcessed = Date(1900, 1, 1);
    }
    payrollRecord.sssPremium_ER = sqlite3_column_int(stmt, 19);
    payrollRecord.philHealthPremium_ER = sqlite3_column_int(stmt, 20);
    payrollRecord.hdmfPremium_ER = sqlite3_column_int(stmt, 21);

    return payrollRecord;
};

// create
sqlite3_int64 PayrollRepository::insertPayroll(const PayrollCalculationResults &prRecord)
{
    const char *sql = R"(INSERT OR IGNORE INTO payroll_records (
            employee_id,
            full_name,
            department,
            pay_period_date,
            pay_period_half,
            basic_salary,
            allowances,
            overtime_pay,
            adjustments,
            gross_income,
            sss_premium_ee,
            philhealth_premium_ee,
            hdmf_premium_ee,
            loan_deductions,
            withholding_tax,
            total_deductions,
            net_pay,
            created_at,
            sss_premium_er, 
            philhealth_premium_er, 
            hdmf_premium_er
        ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("SQL prepare failed: " << sqlite3_errmsg(db));
        return 0;
    }

    PayrollRepository::bindPayroll(stmt, prRecord);
    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_DEBUG("SQL insert failed: " << sqlite3_errmsg(db));
        return 0;
    }

    if (sqlite3_changes(db) == 0)
    {
        LOG_DEBUG("Duplicate payroll record, insert skipped");
        return 0;
    }

    return sqlite3_last_insert_rowid(db);
};
std::vector<PayrollCalculationResults> PayrollRepository::getPayrollByYear(const std::string &payPeriodYear, std::optional<std::string> employeeId, std::optional<int> payPeriodHalf)
{
    // select * FROM payroll_records where pay_period_date like "2025%"
    std::string sql = "SELECT * FROM payroll_records WHERE pay_period_date like ?";
    if (employeeId.has_value())
        sql += " AND employee_id = ?";
    if (payPeriodHalf.has_value())
        sql += " AND pay_period_half = ?";

    sqlite3_stmt *stmt = nullptr;
    std::vector<PayrollCalculationResults> results{};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return results;
    }
    int idx = 1;
    std::string yearPattern = payPeriodYear + "%";
    sqlite3_bind_text(stmt, idx++, yearPattern.c_str(), -1, SQLITE_TRANSIENT);
    if (employeeId.has_value())
    {
        sqlite3_bind_text(stmt, idx++, employeeId.value().c_str(), -1, SQLITE_TRANSIENT);
    }
    if (payPeriodHalf.has_value())
    {
        sqlite3_bind_int(stmt, idx++, payPeriodHalf.value());
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto obj = mapPayroll(stmt);
        results.push_back(obj);
    }

    sqlite3_finalize(stmt);
    return results;
}
std::vector<PayrollCalculationResults> PayrollRepository::getPayrollByPeriod(const std::string &payPeriodDate, std::optional<std::string> employeeId, std::optional<int> payPeriodHalf)
{
    std::string sql = "SELECT * FROM payroll_records WHERE pay_period_date = ?";
    if (employeeId.has_value())
        sql += " AND employee_id = ?";
    if (payPeriodHalf.has_value())
        sql += " AND pay_period_half = ?";

    sqlite3_stmt *stmt = nullptr;
    std::vector<PayrollCalculationResults> results{};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return results;
    }
    int idx = 1;
    sqlite3_bind_text(stmt, idx++, payPeriodDate.c_str(), -1, SQLITE_TRANSIENT);
    if (employeeId.has_value())
    {
        sqlite3_bind_text(stmt, idx++, employeeId.value().c_str(), -1, SQLITE_TRANSIENT);
    }
    if (payPeriodHalf.has_value())
    {
        sqlite3_bind_int(stmt, idx++, payPeriodHalf.value());
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto obj = mapPayroll(stmt);
        results.push_back(obj);
    }

    sqlite3_finalize(stmt);
    return results;
}
std::string PayrollRepository::getLatestPeriod()
{
    const char *sql = "SELECT DISTINCT pay_period_date FROM payroll_records ORDER BY pay_period_date DESC LIMIT 1";
    sqlite3_stmt *stmt = nullptr;
    std::string result{};
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return result;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return result;
}
// read
std::optional<PayrollCalculationResults> PayrollRepository::getById(int id)
{
    const char *sql = "SELECT * FROM payroll_records WHERE id = ?";
    sqlite3_stmt *stmt = nullptr;

    std::optional<PayrollCalculationResults> result = std::nullopt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return result;
    }

    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapPayroll(stmt);
        LOG_DEBUG("Payroll record ID: " << id);
    }

    sqlite3_finalize(stmt);
    return result;
};

std::vector<PayrollCalculationResults> PayrollRepository::getAllById(std::string &id)
{
    std::string sql = "SELECT * FROM payroll_records WHERE employee_id = ?";
    auto results = this->query<PayrollCalculationResults>(sql, mapPayroll, [&id](sqlite3_stmt *stmt)
                                                          { sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT); });
    if (!results.empty())
        return results;
    LOG_DEBUG("Results for getAllById() is empty for employee_id: " << id);
    return {};
}

std::vector<PayrollCalculationResults> PayrollRepository::getAll()
{
    std::string sql = "SELECT * FROM payroll_records";
    auto results = this->query<PayrollCalculationResults>(sql, mapPayroll);
    if (!results.empty())
        return results;
    LOG_DEBUG("result set from getAll() is empty");
    return {};
}

// update
bool PayrollRepository::updatePayroll(const PayrollCalculationResults &prRecord)
{
    std::string sql = "UPDATE payroll_records SET employee_id = ?, full_name = ?, department = ?, pay_period_date = ?, pay_period_half = ?, basic_salary = ?, allowances = ?, overtime_pay = ?, adjustments = ?, gross_income = ?, sss_premium_ee = ?, philhealth_premium_ee = ?, hdmf_premium_ee = ?, loan_deductions = ?, withholding_tax = ?, total_deductions = ?, net_pay = ?, sss_premium_er = ?, philhealth_premium_er = ?, hdmf_premium_er = ? WHERE id = ?";
    return execute(sql, [&prRecord](sqlite3_stmt *stmt)
                   {
        bindPayroll(stmt, prRecord);
        sqlite3_bind_int(stmt, 21, prRecord.id); });
}

// delete
bool PayrollRepository::deletePayroll(int id)
{
    std::string sql = R"(
    delete 
    from payroll_records 
    where id = ?
    )";
    return execute(sql, [&id](sqlite3_stmt *stmt)
                   { sqlite3_bind_int(stmt, 1, id); });
};

int PayrollRepository::getLastPayrollId()
{
    return getAll().size() + 1;
};

std::optional<PayrollConfig> PayrollRepository::loadConfig()
{
    const char *sql = "select * from payroll_config";
    return querySingle<PayrollConfig>(sql, mapPayrollConfig);
}

bool PayrollRepository::saveConfig(const PayrollConfig &config)
{
    const char *sql = "update payroll_config set sss_schedule = ?, philhealth_schedule = ?, hdmf_schedule = ?";
    return execute(sql, [&config](sqlite3_stmt *stmt)
                   {
        sqlite3_bind_int(stmt, 1, static_cast<int>(config.sssSchedule));
        sqlite3_bind_int(stmt, 2, static_cast<int>(config.philHealthSchedule));
        sqlite3_bind_int(stmt, 3, static_cast<int>(config.hdmfSchedule)); });
}
