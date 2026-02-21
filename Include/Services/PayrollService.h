#ifndef PayrollService_H
#define PayrollService_H
#include "Repositories/PayrollRepository.h"

class PayrollService
{
protected:
    PayrollRepository &repo;

public:
    explicit PayrollService(PayrollRepository &r);

    // CREATE
    sqlite3_int64 addPayroll(const PayrollCalculationResults &payrollCalculationResults);

    // READ
    std::vector<PayrollCalculationResults> getAllPayrolls();

    std::vector<PayrollCalculationResults> getAllPayrollsAggregatedToMonthly();

    std::optional<PayrollCalculationResults> getPayrollByID(int id);

    std::vector<PayrollCalculationResults> getAllPayrollsByEmployeeID(std::string &id);

    std::optional<PayrollCalculationResults> getPayrollByEmployeeAndPeriod(const std::string &employeeId, const std::string &payPeriodText, int payPeriodHalf);

    // UPDATE
    bool updatePayroll(const PayrollCalculationResults &e);

    // DELETE
    bool deletePayroll(int id);
};

#endif