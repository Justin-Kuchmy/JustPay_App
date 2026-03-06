#include "Services/GovernmentRemittanceService.h"

GovernmentRemittanceService::GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo) : remittanceRepo(remRepo), payrollRepo(payRepo)
{
    // we have access to PayrollRepository but we need to check which ones have been turned into a governmentremittance object yet,
    // then we get all the ones that havent not been created, then loop through and call generateRemittanceFromPayroll, then we call the repos insertRemittance
}

std::vector<GovernmentRemittance> GovernmentRemittanceService::getRemittancesForWidget(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
}

GovernmentRemittance GovernmentRemittanceService::generateRemittanceFromPayroll(const PayrollCalculationResults &payroll)
{
    GovernmentRemittance rem{};

    // ========================================
    // Copy Reference Data
    // ========================================
    rem.payrollCalculationResultsId = payroll.id;
    rem.employeeId = payroll.employeeId;
    rem.fullName = payroll.fullName;
    rem.employeeDepartment = payroll.employeeDepartment;
    rem.payPeriodText = payroll.payPeriodText;
    rem.payPeriodHalf = payroll.payPeriodHalf;

    // ========================================
    // Copy SSS Contribution Data
    // ========================================
    rem.sssPremium_EE = payroll.sssPremium_EE; // Employee share
    rem.sssPremium_ER = payroll.sssPremium_ER; // Employer share
    rem.sssPremiumTotal = payroll.sssPremium_EE + payroll.sssPremium_ER;

    // ========================================
    // Copy PHIC Contribution Data
    // ========================================
    rem.philHealthPremium_EE = payroll.philHealthPremium_EE; // Employee share
    rem.philHealthPremium_ER = payroll.philHealthPremium_ER; // Employer share
    rem.philHealthPremiumTotal = payroll.philHealthPremium_EE + payroll.philHealthPremium_ER;

    // ========================================
    // Copy HDMF Contribution Data
    // ========================================
    rem.hdmfPremium_EE = payroll.hdmfPremium_EE; // Employee share
    rem.hdmfPremium_ER = payroll.hdmfPremium_ER; // Employer share
    rem.hdmfPremiumTotal = payroll.hdmfPremium_EE + payroll.hdmfPremium_ER;

    // ========================================
    // Copy Withholding Tax
    // ========================================
    rem.withHoldingTax = payroll.withHoldingTax;

    // ========================================
    // Set Initial Submission Status
    // ========================================
    rem.sssSubmissionStatus = RemittanceStatus::PENDING;
    rem.phicSubmissionStatus = RemittanceStatus::PENDING;
    rem.hdmfSubmissionStatus = RemittanceStatus::PENDING;
    rem.withHoldingTaxSubmissionStatus = RemittanceStatus::PENDING;

    // ========================================
    // Set Audit Trail
    // ========================================
    rem.dateCreated = Date::getTodayDate();  // Current date
    rem.dateModified = Date::getTodayDate(); // Current date
    // lastSubmittedDate remains default (1970-01-01)
    // submittedByUserId remains 0 (not yet submitted)

    return rem;
}

std::vector<GovernmentRemittance> GovernmentRemittanceService::generateRemittancesForPeriod(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
}

bool GovernmentRemittanceService::submitRemittancesForPeriod(const std::string &payPeriodText, int payPeriodHalf, const std::string &remittanceType, int userId)
{
    return 0;
}

GovernmentRemittanceRepository::MonthlySummary GovernmentRemittanceService::getMonthlySummary(const std::string &monthYear)
{
    return {};
}

PeriodTotals GovernmentRemittanceService::calculatePeriodTotals(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
}

bool GovernmentRemittanceService::submitRemittance(int remittanceId, const std::string &remittanceType, int submittedByUserId)
{
    return 0;
}

bool GovernmentRemittanceService::confirmRemittance(int remittanceId, const std::string &remittanceType)
{
    return 0;
}

bool GovernmentRemittanceService::rejectRemittance(int remittanceId, const std::string &remittanceType)
{
    return 0;
}

bool GovernmentRemittanceService::isPeriodFullySubmitted(const std::string &payPeriodText, int payPeriodHalf, const std::string &remittanceType)
{
    return 0;
}

bool GovernmentRemittanceService::isPeriodFullyConfirmed(const std::string &payPeriodText, int payPeriodHalf, const std::string &remittanceType)
{
    return 0;
}

PeriodStatusSummary GovernmentRemittanceService::getPeriodStatusSummary(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
}