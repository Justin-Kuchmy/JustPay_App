#ifndef YEARENDBENEFITSERVICE_H
#define YEARENDBENEFITSERVICE_H
#include "Repositories/PayrollRepository.h"
#include "Repositories/LeaveRepository.h"

class YearEndBenefitsService
{
protected:
    PayrollRepository &payrollRepo;
    LeaveRepository &leaveRepo;

public:
    explicit YearEndBenefitsService(PayrollRepository &payrollRepo, LeaveRepository &leaveRepo);
    ~YearEndBenefitsService();
    YearEndBenefits compute(std::string employeeId, int year);
};

#endif