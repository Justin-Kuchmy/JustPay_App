#ifndef PayrollService_H
#define PayrollService_H
#include "Repositories/PayrollRepository.h"
#include "Helpers/PayrollCalculations.h"

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
    // READ
    std::optional<PayrollConfig> loadConfig();

    // UPDATE
    bool updatePayroll(const PayrollCalculationResults &e);

    bool saveConfig(const PayrollConfig &config);

    // DELETE
    bool deletePayroll(int id);

    // calculations
    bool applies(DeductionSchedule schedule, int half);

    double computeSSS(const PayrollCalculationResults &emp);
    double computePhilHealth(const PayrollCalculationResults &emp);
    double computeHDMF(const PayrollCalculationResults &emp);
    double computeTaxableIncome(const PayrollCalculationResults &emp);
    double computeWithholding(double taxableIncome);
    double computeTotalDeductions(const PayrollCalculationResults &emp);
    double computeNetPay(const PayrollCalculationResults &emp);
};

#endif