#include "../../include/modules/ReportsLogsModule.hpp"
#include <iostream>

// ReportsLogsModule
ReportsLogsModule::ReportsLogsModule() 
{
}
ReportsLogsModule::ReportsLogsModule(ActionDispatcher& dispatcher) 
{
    dispatcher.regAction("reports-logs", "view_payroll_registers",  std::bind(&ReportsLogsModule::viewPayrollRegisters, this));
    dispatcher.regAction("reports-logs", "view_payroll_journals", std::bind(&ReportsLogsModule::viewPayrollJournalEntries, this));
    dispatcher.regAction("reports-logs", "view_budget_reports", std::bind(&ReportsLogsModule::generateBudgetUtilizationReport, this));
}
ReportsLogsModule::~ReportsLogsModule() 
{
    std::cout << "ReportsLogsModule destroyed\n";
}
void ReportsLogsModule::viewPayrollRegisters()
{
    std::cout << "ReportsLogsModule::viewPayrollRegisters()" << std::endl;
};
void ReportsLogsModule::viewPayrollJournalEntries()
{
    std::cout << "ReportsLogsModule::viewPayrollJournalEntries()" << std::endl;
};
void ReportsLogsModule::generateBudgetUtilizationReport()
{
    std::cout << "ReportsLogsModule::generateBudgetUtilizationReport()" << std::endl;
};


// GovernmentRemittanceReport
GovernmentReports::GovernmentReports() 
{
}
GovernmentReports::GovernmentReports(ActionDispatcher& dispatcher) 
{
    dispatcher.regAction("gov-remittance", "sss_premium",  std::bind(&GovernmentReports::generateSSSPremiumReport , this));
    dispatcher.regAction("gov-remittance", "phic_premium",  std::bind(&GovernmentReports::generatePHICPremiumReport , this));
    dispatcher.regAction("gov-remittance", "hdmf_premium",  std::bind(&GovernmentReports::generateHDMFPremiumReport , this));
    dispatcher.regAction("gov-remittance", "sss_loans",  std::bind(&GovernmentReports::generateSSSLoanReport , this));
    dispatcher.regAction("gov-remittance", "hdmf_loans", std::bind(&GovernmentReports::generateHDMFLoanReport , this));
    dispatcher.regAction("gov-remittance", "withholding_taxes", std::bind(&GovernmentReports::generateWithholdingTaxReport , this));
}
GovernmentReports::~GovernmentReports() 
{
    std::cout << "GovernmentRemittanceReport destroyed\n";
}
void GovernmentReports::generateSSSPremiumReport()
{
    std::cout << "GovernmentReports::generateSSSPremiumReport()" << std::endl;
};
void GovernmentReports::generatePHICPremiumReport()
{
    std::cout << "GovernmentReports::generatePHICPremiumReport()" << std::endl;
};
void GovernmentReports::generateHDMFPremiumReport()
{
    std::cout << "GovernmentReports::generateHDMFPremiumReport()" << std::endl;
};
void GovernmentReports::generateSSSLoanReport()
{
    std::cout << "GovernmentReports::generateSSSLoanReport()" << std::endl;
};
void GovernmentReports::generateHDMFLoanReport()
{
    std::cout << "GovernmentReports::generateHDMFLoanReport()" << std::endl;
};
void GovernmentReports::generateWithholdingTaxReport()
{
    std::cout << "GovernmentReports::generateWithholdingTaxReport()" << std::endl;
};

// YearEndBenefits
YearEndBenefitsCalculator::YearEndBenefitsCalculator() 
{
}
YearEndBenefitsCalculator::YearEndBenefitsCalculator(ActionDispatcher& dispatcher) 
{
    dispatcher.regAction("yearend-benefits", "compute_13th_month",  std::bind(&YearEndBenefitsCalculator::calculateThirteenthMonthPay, this));
    dispatcher.regAction("yearend-benefits", "monetized_leave", std::bind(&YearEndBenefitsCalculator::calculateMonetizedVacationLeaveCredits , this));
}
YearEndBenefitsCalculator::~YearEndBenefitsCalculator() 
{
    std::cout << "YearEndBenefits destroyed\n";
}
void YearEndBenefitsCalculator::calculateThirteenthMonthPay()
{
    std::cout << "YearEndBenefitsCalculator::calculateThirteenthMonthPay()" << std::endl;
};
void YearEndBenefitsCalculator::calculateMonetizedVacationLeaveCredits()
{
    std::cout << "YearEndBenefitsCalculator::calculateMonetizedVacationLeaveCredits()" << std::endl;
};