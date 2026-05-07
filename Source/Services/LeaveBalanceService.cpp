#include "Services/LeaveBalanceService.h"

LeaveBalanceService::LeaveBalanceService(LeaveRepository &lr, EmployeeRepository &er) : leaveRepo(lr), empRepo(er)
{
}

double LeaveBalanceService::computeLeaveEntitlement(const std::string &employeeId, int year)
{
    constexpr double FULL_ENTITLEMENT = 10.0;
    Date cutOff{year, 11, 30};

    auto empOpt = empRepo.getById(employeeId);
    if (!empOpt.has_value())
        return 0.0;

    Date regularizationDate = empOpt->dateHired.addMonths(6);

    if (isFullyEntitled(regularizationDate, cutOff))
        return FULL_ENTITLEMENT;

    return proRateEntitlement(regularizationDate, cutOff, FULL_ENTITLEMENT);
};

double LeaveBalanceService::computeUnusedLeave(const std::string &employeeId, int year)
{
    double entitlement = computeLeaveEntitlement(employeeId, year);

    auto balanceOpt = leaveRepo.getByEmployeeAndYear(employeeId, year);
    double leaveUsed = balanceOpt.has_value() ? balanceOpt->leaveUsed : 0.0;

    double unused = entitlement - leaveUsed;
    return unused < 0.0 ? 0.0 : unused;
};

EmployeeLeaveBalance LeaveBalanceService::computeLeaveBalance(const std::string &employeeId, int year)
{
    EmployeeLeaveBalance balance{};
    balance.employeeId = employeeId;
    balance.year = year;
    balance.totalLeaveEarned = computeLeaveEntitlement(employeeId, year);

    auto balanceOpt = leaveRepo.getByEmployeeAndYear(employeeId, year);
    balance.leaveUsed = balanceOpt.has_value() ? balanceOpt->leaveUsed : 0.0;
    balance.unusedLeaveDays = computeUnusedLeave(employeeId, year);

    return balance;
};

double LeaveBalanceService::proRateEntitlement(Date regularizationDate, Date cutOffDate, double fullEntitlement)
{
    int daysFromRegToCutOff = regularizationDate.daysUntil(cutOffDate) + 1; // inclusive
    return fullEntitlement * (daysFromRegToCutOff / 365.0);
};
bool LeaveBalanceService::isFullyEntitled(Date regularizationDate, Date cutOffDate)
{
    Date previousCutOff{cutOffDate.year - 1, cutOffDate.month, cutOffDate.day};
    return regularizationDate <= previousCutOff;
};

std::optional<EmployeeLeaveBalance> LeaveBalanceService::getByEmployeeAndYear(const std::string &employeeId, int year)
{
    return leaveRepo.getByEmployeeAndYear(employeeId, year);
}
