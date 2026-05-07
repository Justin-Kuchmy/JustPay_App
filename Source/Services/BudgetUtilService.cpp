#include "Services/BudgetUtilService.h"

BudgetUtilService::BudgetUtilService(PayrollService &p, GovernmentRemittanceService &g) : pService(p), gService(g)
{
}

std::vector<MonthlyBudgetUtilizationReport> BudgetUtilService::getMonthlyBudget(const std::string &period)
{
    std::vector<MonthlyBudgetUtilizationReport> results{};
    std::unordered_map<std::string, MonthlyBudgetUtilizationReport> map{};
    auto payrolls = period.empty() ? pService.getLatestPeriodPayrolls() : pService.getPayrollByPeriod(period);
    double grandTotal{0.0};
    for (const auto &payroll : payrolls)
    {
        grandTotal = 0.0;
        auto key = department_to_string(payroll.employeeDepartment);
        auto &report = map[key];
        report.departmentName = key;
        report.departmentId = payroll.employeeDepartment;
        report.periodLabel = payroll.payPeriodDate;
        report.totalBasicPay += (payroll.monthlyBasicSalary);
        report.totalAdjustments += payroll.adjustments;
        report.total13MonthPay += (payroll.monthlyBasicSalary / 12.0);
        report.totalAllowances += payroll.monthlyAllowances;
        report.total_sssPremium_Employer += payroll.sssPremium_ER;
        report.total_phicPremium_Employer += payroll.philHealthPremium_ER;
        report.total_hdmfPremium_Employer += payroll.hdmfPremium_ER;

        grandTotal += (payroll.monthlyBasicSalary);
        grandTotal += payroll.adjustments;
        grandTotal += payroll.monthlyBasicSalary;
        grandTotal += payroll.monthlyAllowances;
        grandTotal += payroll.sssPremium_ER;
        grandTotal += payroll.philHealthPremium_ER;
        grandTotal += payroll.hdmfPremium_ER;

        report.grandTotal = grandTotal;
    }
    for (const auto &[k, v] : map)
    {
        results.push_back(v);
    }
    // for (const auto &item : results)
    //     LOG_DEBUG(item.to_string() << "\n");
    return results;
}