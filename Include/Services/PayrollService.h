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

    std::vector<PayrollCalculationResults> getPayrollByPeriod(const std::string &payPeriodText, std::optional<std::string> employeeId = std::nullopt, std::optional<int> payPeriodHalf = std::nullopt);

    // UPDATE
    bool updatePayroll(const PayrollCalculationResults &e);

    // DELETE
    bool deletePayroll(int id);
};

#endif