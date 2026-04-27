#include "Services/BudgetUtilService.h"

BudgetUtilService::BudgetUtilService(PayrollService &p, GovernmentRemittanceService &g) : pService(p), gService(g)
{
}

std::vector<MonthlyBudgetUtilizationReport> BudgetUtilService::getMonthlyBudget()
{
    return {};
}