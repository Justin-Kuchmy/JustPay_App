#ifndef LEAVEBALANCESERVICE_H
#define LEAVEBALANCESERVICE_H
#include "Repositories/LeaveRepository.h"
#include "Repositories/EmployeeRepository.h"

class LeaveBalanceService
{
protected:
    LeaveRepository &leaveRepo;
    EmployeeRepository &empRepo;

public:
    explicit LeaveBalanceService(LeaveRepository &leaveRepo, EmployeeRepository &empRepo);

    // Core entitlement — handles full vs pro-rated
    double computeLeaveEntitlement(const std::string &employeeId, int year);

    // Applies entitlement against used leaves, returns unused
    double computeUnusedLeave(const std::string &employeeId, int year);

    // Full balance struct, ready for YearEndBenefitsService to consume
    EmployeeLeaveBalance computeLeaveBalance(const std::string &employeeId, int year);

    std::optional<EmployeeLeaveBalance> getByEmployeeAndYear(const std::string &employeeId, int year);

private:
    double proRateEntitlement(Date regularizationDate, Date cutOffDate, double fullEntitlement);
    bool isFullyEntitled(Date regularizationDate, Date cutOffDate);
};

#endif