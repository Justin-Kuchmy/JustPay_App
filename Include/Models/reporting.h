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
        oss << std::setw(6) << departmentId
            << " | " << std::setw(10) << departmentName
            << " | " << std::setw(13) << periodLabel
            << " | " << std::setw(13) << totalBasicPay
            << " | " << std::setw(13) << totalAdjustments
            << " | " << std::setw(13) << total13MonthPay
            << " | " << std::setw(13) << total_sssPremium_Employer
            << " | " << std::setw(13) << total_phicPremium_Employer
            << " | " << std::setw(13) << total_hdmfPremium_Employer
            << " | " << std::setw(13) << totalSalaries
            << " | " << std::setw(13) << totalAllowances
            << " | " << std::setw(13) << totalPayrollCost;
        return oss.str();
    }

    static std::string header()
    {
        std::ostringstream oss;
        oss << std::setw(6) << "deptId"
            << " | " << std::setw(10) << "deptName"
            << " | " << std::setw(13) << "period"
            << " | " << std::setw(13) << "basicPay"
            << " | " << std::setw(13) << "adjustments"
            << " | " << std::setw(13) << "13monthPay"
            << " | " << std::setw(13) << "sss_ER"
            << " | " << std::setw(13) << "phic_ER"
            << " | " << std::setw(13) << "hdmf_ER"
            << " | " << std::setw(13) << "salaries"
            << " | " << std::setw(13) << "allowances"
            << " | " << std::setw(13) << "totalCost";
        return oss.str();
    }
};

// DTO
struct TaxReconciliationReport
{
    std::string employeeId{};
    std::string employeeName{};

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
};
