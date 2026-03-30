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
YearEndBenefits YearEndBenefitsService::compute(std::string employeeId, int year)
{
    YearEndBenefits yearEnd{};
    yearEnd.employeeId = employeeId;
    yearEnd.year = year;

    std::vector<PayrollCalculationResults> payrolls = payrollRepo.getAllById(employeeId);

    std::erase_if(payrolls, [year](const PayrollCalculationResults &item)
                  { size_t pos = item.payPeriodText.find(' ');
                    return pos == std::string::npos || std::stoi(item.payPeriodText.substr(pos + 1)) != year; });

    for (auto &item : payrolls)
    {
        yearEnd.totalBasicSalary += item.monthlyBasicSalary;
        yearEnd.thirteenthMonthPay += (item.monthlyBasicSalary / 12);
    }

    auto empOpt = empRepo.getById(employeeId);
    double currentBasicSalary = empOpt.has_value() ? empOpt->monthlyBasicSalary : 0.0;

    EmployeeLeaveBalance leaveCredits = leaveBalanceService.computeLeaveBalance(employeeId, year);

    yearEnd.unusedLeaveDays = leaveCredits.unusedLeaveDays;
    yearEnd.dailyRate = currentBasicSalary * 12.0 / 314.0;
    yearEnd.monetizedLeaveValue = yearEnd.dailyRate * yearEnd.unusedLeaveDays;

    return yearEnd;
}