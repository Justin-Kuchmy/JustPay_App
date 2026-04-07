#pragma once
#include "Models/Primitives/enums.h"
#include "Models/Primitives/date.h"
#include <string>

struct YearEndBenefits
{
    std::string employeeId{""};
    int year{1970};

    double totalBasicSalary{0.0};

    double thirteenthMonthPay{0.0};

    double unusedLeaveDays{0.0};
    double dailyRate{0.0};
    double monetizedLeaveValue{0.0};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "employeeId:  " << employeeId
            << "\n year: " << year
            << "\n totalBasicSalary: " << totalBasicSalary
            << "\n thirteenthMonthPay: " << thirteenthMonthPay
            << "\n unusedLeaveDays: " << unusedLeaveDays
            << "\n dailyRate: " << dailyRate
            << "\n monetizedLeaveValue: " << monetizedLeaveValue;

        return oss.str();
    }
};

struct PayrollConfig
{
    DeductionSchedule sssSchedule{DeductionSchedule::SecondHalf};
    DeductionSchedule philHealthSchedule{DeductionSchedule::SecondHalf};
    DeductionSchedule hdmfSchedule{DeductionSchedule::SecondHalf};
};

class PayrollCalculationResults
{
public:
    int id{};
    std::string employeeId{""};
    std::string fullName{""};
    std::string employeeDepartment{""};
    std::string payPeriodText{""};
    int payPeriodHalf{1};
    double monthlyBasicSalary{0.0};
    double monthlyAllowances{0.0};
    double overTimePay{0.0};
    double adjustments{0.0};
    double grossIncome{0.0};
    double sssPremium_EE{0.0};
    double philHealthPremium_EE{0.0};
    double hdmfPremium_EE{0.0};
    double loanDeductionsPerPayroll{0.0};
    bool applySSS{false};
    bool applyPhilHealth{false};
    bool applyHDMF{false};
    double withHoldingTax{0.0};
    double totalDeductions{0.0};
    double netPay{0.0};
    Date dateProcessed{Date(2025, 12, 18)};
    double sssPremium_ER{0.0};
    double philHealthPremium_ER{0.0};
    double hdmfPremium_ER{0.0};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "PayrollCalculationResults { "
            << "\n dateProcessed: " << dateProcessed.to_string()
            << "\n employeeId: " << employeeId
            << "\n fullName: " << fullName
            << "\n payPeriodText: " << payPeriodText
            << "\n payPeriodHalf: " << payPeriodHalf
            << "\n employeeDepartment: " << employeeDepartment
            << "\n monthlyBasicSalary: " << monthlyBasicSalary
            << "\n monthlyAllowances: " << monthlyAllowances
            << "\n overTimePay: " << overTimePay
            << "\n adjustments: " << adjustments
            << "\n grossIncome: " << grossIncome
            << "\n sssPremium_EE: " << sssPremium_EE
            << "\n philHealthPremium_EE: " << philHealthPremium_EE
            << "\n hdmfPremium_EE: " << hdmfPremium_EE
            << "\n sssPremium_ER: " << sssPremium_ER
            << "\n philHealthPremium_ER: " << philHealthPremium_ER
            << "\n hdmfPremium_ER: " << hdmfPremium_ER
            << "\n loanDeductionsPerPayroll: " << loanDeductionsPerPayroll
            << "\n totalDeductions: " << totalDeductions
            << "\n withHoldingTax: " << withHoldingTax
            << "\n netPay: " << netPay
            << " }";
        return oss.str();
    };
};
