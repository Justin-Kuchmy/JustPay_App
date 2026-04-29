#ifndef BUDGETUTILSERVICE_H
#define BUDGETUTILSERVICE_H
#include "Services/PayrollService.h"
#include "Services/GovernmentRemittanceService.h"
#include "Models/reporting.h"

class BudgetUtilService
{
protected:
    PayrollService &pService;
    GovernmentRemittanceService &gService;

public:
    explicit BudgetUtilService(PayrollService &pService, GovernmentRemittanceService &gService);
    std::vector<MonthlyBudgetUtilizationReport> getMonthlyBudget(const std::string &period = "");
};
#endif