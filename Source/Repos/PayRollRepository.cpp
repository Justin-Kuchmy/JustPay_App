#include "Repositories/PayrollRepository.h"
#include <format>
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
    sqlite3_bind_text(stmt, idx++, prRecord.employeeDepartment.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, prRecord.payPeriodText.c_str(), -1, SQLITE_TRANSIENT);
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

    const unsigned char *employeeDepartment = sqlite3_column_text(stmt, 3);
    payrollRecord.employeeDepartment = employeeDepartment ? reinterpret_cast<const char *>(employeeDepartment) : std::string{};

    const unsigned char *payPeriodText = sqlite3_column_text(stmt, 4);
    payrollRecord.payPeriodText = payPeriodText ? reinterpret_cast<const char *>(payPeriodText) : std::string{};

    payrollRecord.payPeriodHalf = sqlite3_column_int(stmt, 5);

    payrollRecord.monthlyBasicSalary = sqlite3_column_int(stmt, 6);
    payrollRecord.monthlyAllowances = sqlite3_column_int(stmt, 7);
    payrollRecord.overTimePay = sqlite3_column_int(stmt, 8);
    payrollRecord.adjustments = sqlite3_column_int(stmt, 9);
    payrollRecord.grossIncome = sqlite3_column_int(stmt, 10);
    payrollRecord.sssPremium_EE = sqlite3_column_int(stmt, 11);
    payrollRecord.philHealthPremium_EE = sqlite3_column_int(stmt, 12);
    payrollRecord.hdmfPremium_EE = sqlite3_column_int(stmt, 13);
    payrollRecord.loanDeductionsPerPayroll = sqlite3_column_int(stmt, 14);
    payrollRecord.withHoldingTax = sqlite3_column_int(stmt, 15);
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
            pay_period_text,
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
std::vector<PayrollCalculationResults> PayrollRepository::getPayrollByPeriod(const std::string &payPeriodText, std::optional<std::string> employeeId, std::optional<int> payPeriodHalf)
{
    std::string sql = "SELECT * FROM payroll_records WHERE pay_period_text = ?";
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
    sqlite3_bind_text(stmt, idx++, payPeriodText.c_str(), -1, SQLITE_TRANSIENT);
    LOG_DEBUG("payPeriodText");
    if (employeeId.has_value())
    {
        sqlite3_bind_text(stmt, idx++, employeeId.value().c_str(), -1, SQLITE_TRANSIENT);
        LOG_DEBUG("employeeId");
    }
    if (payPeriodHalf.has_value())
    {
        sqlite3_bind_int(stmt, idx++, payPeriodHalf.value());
        LOG_DEBUG("payPeriodHalf");
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto obj = mapPayroll(stmt);
        results.push_back(obj);
    }

    sqlite3_finalize(stmt);
    return results;
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
    std::string sql = std::format(
        "SELECT * FROM payroll_records WHERE employee_id = '{}'",
        id);

    auto results = this->query<PayrollCalculationResults>(sql, mapPayroll);

    if (!results.empty())
    {
        return results;
    }
    else
    {
        LOG_DEBUG("Failed to get payroll records from the db for employee_id: " << id);
        return {};
    }
};

std::vector<PayrollCalculationResults> PayrollRepository::getAll()
{
    std::string sql = std::format("SELECT * FROM payroll_records");

    auto results = this->query<PayrollCalculationResults>(sql, mapPayroll);

    if (!results.empty())
    {
        return results;
    }
    else
    {
        LOG_DEBUG("Failed to get payroll records from the db");
        return {};
    }
};

// update
bool PayrollRepository::updatePayroll(const PayrollCalculationResults &prRecord)
{
    std::string sql = std::format(
        "UPDATE payroll_records SET employee_id = '{}', full_name = '{}', department = '{}', pay_period_text = '{}', pay_period_half = {}, basic_salary = {}, allowances = {}, overtime_pay = {}, adjustments = {}, gross_income = {}, sss_premium_ee = {}, philhealth_premium_ee = {}, hdmf_premium_ee = {}, loan_deductions = {}, withholding_tax = {}, total_deductions = {}, net_pay = {}, sss_premium_er = {}, philhealth_premium_er = {}, hdmf_premium_er = {} WHERE id = {}",
        prRecord.employeeId,
        prRecord.fullName,
        prRecord.employeeDepartment,
        prRecord.payPeriodText,
        prRecord.payPeriodHalf,
        prRecord.monthlyBasicSalary,
        prRecord.monthlyAllowances,
        prRecord.overTimePay,
        prRecord.adjustments,
        prRecord.grossIncome,
        prRecord.sssPremium_EE,
        prRecord.philHealthPremium_EE,
        prRecord.hdmfPremium_EE,
        prRecord.loanDeductionsPerPayroll,
        prRecord.withHoldingTax,
        prRecord.totalDeductions,
        prRecord.netPay,
        prRecord.sssPremium_ER,
        prRecord.philHealthPremium_ER,
        prRecord.hdmfPremium_ER,
        prRecord.id);

    return execute(sql);
};

// delete
bool PayrollRepository::deletePayroll(int id)
{
    std::string sql = R"(
    delete 
    from payroll_records 
    where id = ?
    )";
    return execute(sql, [&id](sqlite3_stmt *stmt) {});
};

int PayrollRepository::getLastPayrollId()
{
    return getAll().size() + 1;
};

std::optional<PayrollConfig> PayrollRepository::loadConfig()
{
    const char *sql = "select * from payroll_config";
    sqlite3_stmt *stmt = nullptr;
    std::optional<PayrollConfig> result = std::nullopt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return result;
    }

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapPayrollConfig(stmt);
    }

    sqlite3_finalize(stmt);
    return result;
}

bool PayrollRepository::saveConfig(const PayrollConfig &config)
{
    // static_cast<int>(config.sssSchedule), static_cast<int>(config.philHealthSchedule), static_cast<int>(config.hdmfSchedule));
    std::string sql = R"(
        update Payroll_Config 
        set sss_schedule = ?, 
        philhealth_schedule = ?, 
        hdmf_schedule = ?
        )";
    return execute(sql, [&config](sqlite3_stmt *stmt) {

    });
}
