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

std::vector<PayrollCalculationResults> PayrollService::getAllPayrolls(std::string id)
{
    return this->repo.getAllById(id);
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