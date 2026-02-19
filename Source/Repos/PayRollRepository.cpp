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
    payrollRecord.sssPremium = sqlite3_column_int(stmt, 11);
    payrollRecord.philHealthPremium = sqlite3_column_int(stmt, 12);
    payrollRecord.hdmfPremium = sqlite3_column_int(stmt, 13);
    payrollRecord.loanDeductionsPerPayroll = sqlite3_column_int(stmt, 14);
    payrollRecord.withHoldingTax = sqlite3_column_int(stmt, 15);
    payrollRecord.totalDeductions = sqlite3_column_int(stmt, 16);
    payrollRecord.netPay = sqlite3_column_int(stmt, 17);
    const unsigned char *date = sqlite3_column_text(stmt, 18);
    if (date)
    {
        payrollRecord.dateProcessed = Date::fromString(reinterpret_cast<const char *>(date));
    }
    else
    {
        payrollRecord.dateProcessed = Date(1900, 1, 1);
    }

    return payrollRecord;
};

// create
sqlite3_int64 PayrollRepository::insertPayroll(const PayrollCalculationResults &prRecord)
{
    const char *sql = R"(INSERT INTO payroll_records (
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
            sss_premium,
            philhealth_premium,
            hdmf_premium,
            loan_deductions,
            withholding_tax,
            total_deductions,
            net_pay
        ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("SQL prepare failed: " << sqlite3_errmsg(db));
        return 0;
    }

    // derive pay period number from payPeriodText string
    int payPeriodNumber = (prRecord.payPeriodText == "first") ? 1 : 2;

    int idx = 1;
    sqlite3_bind_text(stmt, idx++, prRecord.employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, prRecord.fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, prRecord.employeeDepartment.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, prRecord.payPeriodText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, payPeriodNumber);

    sqlite3_bind_double(stmt, idx++, prRecord.monthlyBasicSalary);
    sqlite3_bind_double(stmt, idx++, prRecord.monthlyAllowances);
    sqlite3_bind_double(stmt, idx++, prRecord.overTimePay);
    sqlite3_bind_double(stmt, idx++, prRecord.adjustments);
    sqlite3_bind_double(stmt, idx++, prRecord.grossIncome);

    sqlite3_bind_double(stmt, idx++, prRecord.sssPremium);
    sqlite3_bind_double(stmt, idx++, prRecord.philHealthPremium);
    sqlite3_bind_double(stmt, idx++, prRecord.hdmfPremium);
    sqlite3_bind_double(stmt, idx++, prRecord.loanDeductionsPerPayroll);

    sqlite3_bind_double(stmt, idx++, prRecord.withHoldingTax);
    sqlite3_bind_double(stmt, idx++, prRecord.totalDeductions);
    sqlite3_bind_double(stmt, idx++, prRecord.netPay);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_DEBUG("SQL insert failed: " << sqlite3_errmsg(db));
        return 0;
    }

    return sqlite3_last_insert_rowid(db);
};

std::optional<PayrollCalculationResults> PayrollRepository::getPayrollByEmployeeAndPeriod(const std::string &employeeId, const std::string &payPeriodText, int payPeriodHalf)
{
    const char *sql = "SELECT * FROM payroll_records WHERE employee_id = ? AND pay_period_text = ? AND pay_period_half = ?";
    sqlite3_stmt *stmt = nullptr;

    std::optional<PayrollCalculationResults> result = std::nullopt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return result;
    }

    sqlite3_bind_text(stmt, 1, employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, payPeriodText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, payPeriodHalf);

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapPayroll(stmt);
        LOG_DEBUG("Payroll record for Employee ID: " << employeeId << " for period: " << payPeriodText << " half: " << payPeriodHalf);
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
        "UPDATE payroll_records SET employee_id = '{}', full_name = '{}', department = '{}', pay_period_text = '{}', pay_period_half = {}, basic_salary = {}, allowances = {}, overtime_pay = {}, adjustments = {}, gross_income = {}, sss_premium = {}, philhealth_premium = {}, hdmf_premium = {}, loan_deductions = {}, withholding_tax = {}, total_deductions = {}, net_pay = {} WHERE id = {}",
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
        prRecord.sssPremium,
        prRecord.philHealthPremium,
        prRecord.hdmfPremium,
        prRecord.loanDeductionsPerPayroll,
        prRecord.withHoldingTax,
        prRecord.totalDeductions,
        prRecord.netPay,
        prRecord.id // make sure your entity has an 'id' field!
    );

    return execute(sql);
};

// delete
bool PayrollRepository::deletePayroll(int id)
{
    LOG_DEBUG("PayrollRepository::deletePayroll not implemented");
    return false;
};

std::string PayrollRepository::getLastPayrollId()
{
    LOG_DEBUG("PayrollRepository::getLastPayrollId not implemented");
    return "";
};
