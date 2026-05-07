#include "Services/TaxReconciliationService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

TaxReconciliationService::TaxReconciliationService(EmployeeService &e, PayrollService &p, YearEndBenefitsService &y, GovernmentRemittanceService &g) : employeeService(e), payrollService(p), yearEndService(y), govtRemittService(g)
{
}
TaxReconciliationService::~TaxReconciliationService()
{
}
TaxReconciliationReport TaxReconciliationService::buildDTO(const Employee &employee, const std::vector<PayrollCalculationResults> &payrolls, const YearEndBenefits &yearEnd, const std::vector<GovernmentRemittance> &remittances)
{

    TaxReconciliationReport report{};

    report.employeeId = employee.employeeId;
    report.employeeName = employee.fullName;
    report.tin = employee.tin;
    for (const auto &p : payrolls)
    {
        report.grossIncome += p.grossIncome;
        report.basicPay += p.monthlyBasicSalary;
        report.overtimePay += p.overTimePay;
        report.deMinimis += p.monthlyAllowances;
        report.taxWithheld += p.withHoldingTax;
    }

    for (const auto &r : remittances)
        report.govtContributions += r.employee_Contrib;

    report.thirteenthMonthAndLeave = yearEnd.thirteenthMonthPay + yearEnd.monetizedLeaveValue;
    constexpr double THIRTEENTH_MONTH_EXEMPT = 90000.0;
    constexpr double DE_MINIMIS_EXEMPT = 73100.0;
    report.taxable13thMonth = std::max(0.0, report.thirteenthMonthAndLeave - THIRTEENTH_MONTH_EXEMPT);
    report.taxableDeMinimis = std::max(0.0, report.deMinimis - DE_MINIMIS_EXEMPT);

    // ── Computed totals ─────────────────────────────────────────────
    report.nonTaxableTotal = report.thirteenthMonthAndLeave + report.deMinimis + report.govtContributions;
    report.taxableTotal = report.basicPay + report.overtimePay + report.taxable13thMonth + report.taxableDeMinimis;
    report.taxDue = PayrollCalc::calcAnnualTaxDue(report.taxableTotal);
    report.taxVariance = report.taxDue - report.taxWithheld;
    return report;
}

std::vector<TaxReconciliationReport> TaxReconciliationService::getReconciliation(int year)
{
    auto vecEmployees = employeeService.getAllEmployees();
    std::vector<TaxReconciliationReport> resultsSet{};
    for (const auto &emp : vecEmployees)
    {
        auto vecPayrollsByYear = payrollService.getPayrollByYear(std::to_string(year), emp.employeeId);
        if (vecPayrollsByYear.size() == 0)
            return {};
        auto vecYearEndBenefits = yearEndService.compute();
        auto vecRemits = govtRemittService.createFromPayroll(vecPayrollsByYear);
        auto yearEndBenefitForEmployee = std::find_if(vecYearEndBenefits.begin(), vecYearEndBenefits.end(), [&emp](YearEndBenefits yearEnd)
                                                      { return yearEnd.employeeId == emp.employeeId; });
        auto Dto = buildDTO(emp, vecPayrollsByYear, *yearEndBenefitForEmployee, vecRemits);
        resultsSet.push_back(Dto);
    }
    return resultsSet;
}
