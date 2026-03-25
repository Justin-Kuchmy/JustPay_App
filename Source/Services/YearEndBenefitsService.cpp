#include "Services/YearEndBenefitsService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include <algorithm>
#include <ranges>
#include <QDate>

YearEndBenefitsService::YearEndBenefitsService(PayrollRepository &r, LeaveRepository &l) : payrollRepo(r), leaveRepo(l)
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
                  {size_t pos = item.payPeriodText.find(' '); 
                return pos == std::string::npos || std::stoi(item.payPeriodText.substr(pos + 1)) != year; });
    for (auto &item : payrolls)
    {
        yearEnd.totalBasicSalary += item.monthlyBasicSalary;
        yearEnd.thirteenthMonthPay += (item.monthlyBasicSalary / 12);
    }
    return yearEnd;
}