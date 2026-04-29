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
    int employeeDepartment{0};
    std::string payPeriodDate{""};
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
        oss << std::fixed << std::setprecision(2);
        oss << std::setw(10) << dateProcessed.to_string()
            << " | " << std::setw(8) << employeeId
            << " | " << std::setw(16) << fullName
            << " | " << std::setw(13) << payPeriodDate
            << " | " << std::setw(4) << payPeriodHalf
            << " | " << std::setw(6) << employeeDepartment
            << " | " << std::setw(13) << monthlyBasicSalary
            << " | " << std::setw(13) << monthlyAllowances
            << " | " << std::setw(13) << overTimePay
            << " | " << std::setw(13) << adjustments
            << " | " << std::setw(13) << grossIncome
            << " | " << std::setw(13) << sssPremium_EE
            << " | " << std::setw(13) << philHealthPremium_EE
            << " | " << std::setw(13) << hdmfPremium_EE
            << " | " << std::setw(13) << sssPremium_ER
            << " | " << std::setw(13) << philHealthPremium_ER
            << " | " << std::setw(13) << hdmfPremium_ER
            << " | " << std::setw(13) << loanDeductionsPerPayroll
            << " | " << std::setw(13) << totalDeductions
            << " | " << std::setw(13) << withHoldingTax
            << " | " << std::setw(13) << netPay;
        return oss.str();
    }

    static std::string header()
    {
        std::ostringstream oss;
        oss << std::setw(10) << "date"
            << " | " << std::setw(8) << "empId"
            << " | " << std::setw(16) << "fullName"
            << " | " << std::setw(13) << "period"
            << " | " << std::setw(4) << "half"
            << " | " << std::setw(6) << "dept"
            << " | " << std::setw(13) << "basicSalary"
            << " | " << std::setw(13) << "allowances"
            << " | " << std::setw(13) << "overtimePay"
            << " | " << std::setw(13) << "adjustments"
            << " | " << std::setw(13) << "grossIncome"
            << " | " << std::setw(13) << "sss_EE"
            << " | " << std::setw(13) << "philHealth_EE"
            << " | " << std::setw(13) << "hdmf_EE"
            << " | " << std::setw(13) << "sss_ER"
            << " | " << std::setw(13) << "philHealth_ER"
            << " | " << std::setw(13) << "hdmf_ER"
            << " | " << std::setw(13) << "loanDeduct"
            << " | " << std::setw(13) << "totalDeduct"
            << " | " << std::setw(13) << "withholding"
            << " | " << std::setw(13) << "netPay";
        return oss.str();
    }
};
