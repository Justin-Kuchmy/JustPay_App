#ifndef TAXRECONCILLATIONSERVICE_H
#define TAXRECONCILLATIONSERVICE_H
#include "Models/reporting.h"
#include "Services/EmployeeService.h"
#include "Services/PayrollService.h"
#include "Services/YearEndBenefitsService.h"
#include "Services/GovernmentRemittanceService.h"

class TaxReconciliationService
{

protected:
    EmployeeService &employeeService;
    PayrollService &payrollService;
    YearEndBenefitsService &yearEndService;
    GovernmentRemittanceService &govtRemittService;

    TaxReconciliationReport buildDTO(const Employee &employee, const PayrollCalculationResults &payrolls, const YearEndBenefits &yearEnd, const GovernmentRemittance &remittance);

    double computeNonTaxableTotal(const TaxReconciliationReport &dto);
    double computeTaxableTotal(const TaxReconciliationReport &dto);
    double computeTaxVariance(const TaxReconciliationReport &dto);

public:
    explicit TaxReconciliationService(EmployeeService &employeeService, PayrollService &payrollService, YearEndBenefitsService &yearEndService, GovernmentRemittanceService &govtRemittService);
    ~TaxReconciliationService();

    std::vector<TaxReconciliationReport> getReconciliation(int year);
};

#endif