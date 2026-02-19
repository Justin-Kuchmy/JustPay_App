#include "Services/PayrollService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

PayrollService::PayrollService(PayrollRepository &r) : repo(r)
{
}

// CREATE
sqlite3_int64 PayrollService::addPayroll(const PayrollCalculationResults &Payroll)
{
    return this->repo.insertPayroll(Payroll);
}

// READ
std::optional<PayrollCalculationResults> PayrollService::getPayrollByID(int id)
{
    return this->repo.getById(id);
}

std::vector<PayrollCalculationResults> PayrollService::getAllPayrolls()
{
    return this->repo.getAll();
}

std::vector<PayrollCalculationResults> PayrollService::getAllPayrollsByEmployeeID(std::string &id)
{
    return this->repo.getAllById(id);
}

std::optional<PayrollCalculationResults> PayrollService::getPayrollByEmployeeAndPeriod(const std::string &employeeId, const std::string &payPeriodText, int payPeriodHalf)
{
    return this->repo.getPayrollByEmployeeAndPeriod(employeeId, payPeriodText, payPeriodHalf);
}

// UPDATE
bool PayrollService::updatePayroll(const PayrollCalculationResults &e)
{
    return this->repo.updatePayroll(e);
}

// DELETE
bool PayrollService::deletePayroll(int id)
{
    return this->repo.deletePayroll(id);
}