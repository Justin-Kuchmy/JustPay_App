#pragma once
#include "Models/Primitives/enums.h"
#include "Models/Primitives/date.h"

struct DepartmentBudget
{
    int id{};
    Department department{};
    int period_Id{};

    double allocatedAmount{0.0};
    std::string notes{};
};

// DTO
struct MonthlyBudgetUtilizationReport
{
    int departmentId{};
    std::string departmentName{};
    std::string periodLabel{}; // e.g. "Jan 2026"

    double totalSalaries{0.0};
    double totalAllowances{0.0};
    double totalGovRemittances{0.0};

    double totalPayrollCost{0.0};

    // budget (from DepartmentBudget)
    double allocatedBudget{0.0};

    double variance{0.0};
};

// DTO
struct TaxReconciliationReport
{
    std::string employeeId{};

    double annualTaxableIncome{0.0};
    double computedTaxDue{0.0};
    double totalTaxWithheld{0.0};

    double variance{0.0};
};
