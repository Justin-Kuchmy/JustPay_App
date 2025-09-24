#include "../../include/modules/ReportsLogsModule.hpp"
#include <iostream>

// ReportsLogsModule
ReportsLogsModule::ReportsLogsModule() 
{
    std::cout << "ReportsLogsModule created\n";
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
    std::cout << "GovernmentRemittanceReport created\n";
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
    std::cout << "YearEndBenefits created\n";
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