#include "Services/YearEndBenefitsService.h"
#include "Services/LeaveBalanceService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include <algorithm>
#include <ranges>
#include <QDate>

YearEndBenefitsService::YearEndBenefitsService(PayrollRepository &r, LeaveBalanceService &l, EmployeeRepository &er) : payrollRepo(r), leaveBalanceService(l), empRepo(er)
{
}
YearEndBenefitsService::~YearEndBenefitsService()
{
}
std::vector<YearEndBenefits> YearEndBenefitsService::compute()
{
    std::vector<PayrollCalculationResults> payrolls = payrollRepo.getAll();
    std::map<std::pair<std::string, int>, YearEndBenefits> benefitsMap;

    for (const auto &roll : payrolls)
    {
        size_t pos = roll.payPeriodDate.find(' ');
        int year = std::stoi(roll.payPeriodDate.substr(0, pos));
        auto key = std::make_pair(roll.employeeId, year);

        auto &yearEnd = benefitsMap[key];
        yearEnd.employeeId = roll.employeeId;
        yearEnd.year = year;

        yearEnd.totalBasicSalary += roll.monthlyBasicSalary;
        yearEnd.thirteenthMonthPay += (roll.monthlyBasicSalary / 12);
    }
    std::vector<YearEndBenefits> benefitsForAll{};
    for (auto &[key, yearEnd] : benefitsMap)
    {

        auto empOpt = empRepo.getById(yearEnd.employeeId);
        double currentBasicSalary = empOpt.has_value() ? empOpt->monthlyBasicSalary : 0.0;

        EmployeeLeaveBalance leaveCredits = leaveBalanceService.computeLeaveBalance(yearEnd.employeeId, yearEnd.year);

        yearEnd.unusedLeaveDays = leaveCredits.unusedLeaveDays;
        yearEnd.dailyRate = currentBasicSalary * 12.0 / 314.0;
        yearEnd.monetizedLeaveValue = yearEnd.dailyRate * yearEnd.unusedLeaveDays;

        benefitsForAll.push_back(yearEnd);
    }
    return benefitsForAll;
}