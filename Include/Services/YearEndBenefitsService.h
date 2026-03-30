#ifndef YEARENDBENEFITSERVICE_H
#define YEARENDBENEFITSERVICE_H
#include "Repositories/PayrollRepository.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/LeaveBalanceService.h"

class YearEndBenefitsService
{
protected:
    PayrollRepository &payrollRepo;
    LeaveBalanceService &leaveBalanceService;
    EmployeeRepository &empRepo;

public:
    explicit YearEndBenefitsService(PayrollRepository &payrollRepo, LeaveBalanceService &leaveBalanceService, EmployeeRepository &empRepo);
    ~YearEndBenefitsService();
    YearEndBenefits compute(std::string employeeId, int year);
};

#endif