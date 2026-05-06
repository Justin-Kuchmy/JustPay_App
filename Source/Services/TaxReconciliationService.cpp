#include "Services/TaxReconciliationService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

TaxReconciliationService::TaxReconciliationService(EmployeeService &e, PayrollService &p, YearEndBenefitsService &y, GovernmentRemittanceService &g) : employeeService(e), payrollService(p), yearEndService(y), govtRemittService(g)
{
}
TaxReconciliationService::~TaxReconciliationService()
{
}
TaxReconciliationReport TaxReconciliationService::buildDTO(const Employee &employee, const PayrollCalculationResults &payrolls, const YearEndBenefits &yearEnd, const GovernmentRemittance &remittance)
{
    return {};
}

double TaxReconciliationService::computeNonTaxableTotal(const TaxReconciliationReport &dto)
{
    return 0.0;
};
double TaxReconciliationService::computeTaxableTotal(const TaxReconciliationReport &dto)
{
    return 0.0;
};
double TaxReconciliationService::computeTaxVariance(const TaxReconciliationReport &dto)
{
    return 0.0;
};

std::vector<TaxReconciliationReport> TaxReconciliationService::getReconciliation(int year)
{
    return {};
}