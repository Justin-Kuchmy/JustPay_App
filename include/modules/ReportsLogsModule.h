#pragma once
#include <iostream>
#include "../ActionDispatcher.h"

class ReportsLogsModule {
public:
    ReportsLogsModule();
    ReportsLogsModule(ActionDispatcher& dispatcher);
    ~ReportsLogsModule();
    void viewPayrollRegisters();
    void viewPayrollJournalEntries();
    void generateBudgetUtilizationReport();
};

class GovernmentReports {
public:
    GovernmentReports();
    GovernmentReports(ActionDispatcher& dispatcher);
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
    YearEndBenefitsCalculator(ActionDispatcher& dispatcher);
    ~YearEndBenefitsCalculator();
    void calculateThirteenthMonthPay();
    void calculateMonetizedVacationLeaveCredits();
};
