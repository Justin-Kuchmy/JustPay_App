#pragma once
#include <iostream>

class ReportsLogsModule {
public:
    ReportsLogsModule();
    ~ReportsLogsModule();
    void viewPayrollRegisters();
    void viewPayrollJournalEntries();
    void generateBudgetUtilizationReport();
};

class GovernmentReports {
public:
    GovernmentReports();
    ~GovernmentReports();
    void generateSSSPremiumReport();
    void generatePHICPremiumReport();
    void generateHDMFPremiumReport();
    void generateSSSLoanReport();
    void generateHDMFLoanReport();
    void generateWithholdingTaxReport();
};

class YearEndBenefitsCalculator {
public:
    YearEndBenefitsCalculator();
    ~YearEndBenefitsCalculator();
    void calculateThirteenthMonthPay();
    void calculateMonetizedVacationLeaveCredits();
};
