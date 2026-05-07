#pragma once
#include "Models/Primitives/enums.h"
#include "Models/Primitives/date.h"

struct DepartmentBudget
{
    int id{};
    Department department{};
    int period_Id{};

    double allocatedAmount{0.0};
    std::string notes{};
};

// DTO
struct MonthlyBudgetUtilizationReport
{
    int departmentId{};
    std::string departmentName{};
    std::string periodLabel{}; // e.g. "Jan 2026"

    double totalBasicPay{0.0};
    double totalAdjustments{0.0};
    double total13MonthPay{0.0};
    double total_sssPremium_Employer{0.0};
    double total_phicPremium_Employer{0.0};
    double total_hdmfPremium_Employer{0.0};
    double totalSalaries{0.0};
    double totalAllowances{0.0};
    double totalPayrollCost{0.0};
    double grandTotal{0.0};

    // budget (from DepartmentBudget)
    double allocatedBudget{0.0};
    double variance{0.0};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << "departmentId: " << departmentId
            << " \n departmentName: " << departmentName
            << "\n periodLabel" << periodLabel
            << "\n totalBasicPay   " << totalBasicPay
            << "\n totalAdjustments   " << totalAdjustments
            << "\n total13MonthPay   " << total13MonthPay
            << "\n total_sssPremium_Employer   " << total_sssPremium_Employer
            << "\n total_phicPremium_Employer   " << total_phicPremium_Employer
            << "\n total_hdmfPremium_Employer   " << total_hdmfPremium_Employer
            << "\n totalSalaries   " << totalSalaries
            << "\n totalAllowances   " << totalAllowances
            << "\n totalPayrollCost   " << totalPayrollCost;
        return oss.str();
    }
};

// DTO
struct TaxReconciliationReport
{
    std::string employeeId{};
    std::string employeeName{};
    std::string tin{};

    double grossIncome{0.0};
    double thirteenthMonthAndLeave{0.0};
    double deMinimis{0.0};
    double govtContributions{0.0};
    double nonTaxableTotal{0.0}; // computed

    double basicPay{0.0};
    double taxable13thMonth{0.0};
    double overtimePay{0.0};
    double taxableDeMinimis{0.0};
    double taxableTotal{0.0}; // computed

    double taxDue{0.0};
    double taxWithheld{0.0};
    double taxVariance{0.0}; // computed

    std::string to_string()
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2)
            << "employeeId " << std::setw(20) << employeeId
            << "\nemployeeName " << std::setw(30) << employeeName
            << "\ntin " << std::setw(15) << tin
            << "\ngrossIncome " << std::setw(13) << grossIncome
            << "\nthirteenthMonthAndLeave " << std::setw(13) << thirteenthMonthAndLeave
            << "\ndeMinimis " << std::setw(13) << deMinimis
            << "\ngovtContributions " << std::setw(13) << govtContributions
            << "\nnonTaxableTotal " << std::setw(13) << nonTaxableTotal
            << "\nbasicPay " << std::setw(13) << basicPay
            << "\ntaxable13thMonth " << std::setw(13) << taxable13thMonth
            << "\novertimePay " << std::setw(13) << overtimePay
            << "\ntaxableDeMinimis " << std::setw(13) << taxableDeMinimis
            << "\ntaxableTotal " << std::setw(13) << taxableTotal
            << "\ntaxDue " << std::setw(13) << taxDue
            << "\ntaxWithheld " << std::setw(13) << taxWithheld
            << "\ntaxVariance " << std::setw(13) << taxVariance;
        return oss.str();
    }
};
