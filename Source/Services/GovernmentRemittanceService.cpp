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

bool GovernmentRemittanceService::generateRemittanceFromPayrolls(const std::vector<PayrollCalculationResults> *payrolls)
{
    std::vector<GovernmentRemittance> v_Rem{};
    for (size_t i{0}; i < payrolls->size(); i++)
    {
        GovernmentRemittance rem{};
        auto &payroll = payrolls->at(i);
        rem.payrollCalculationResultsId = payroll.id;
        rem.employeeId = payroll.employeeId;
        rem.fullName = payroll.fullName;
        rem.employeeDepartment = payroll.employeeDepartment;
        rem.payPeriodText = payroll.payPeriodText;
        rem.payPeriodHalf = payroll.payPeriodHalf;

        rem.sssPremium_EE = payroll.sssPremium_EE; // Employee share
        rem.sssPremium_ER = payroll.sssPremium_ER; // Employer share
        rem.sssPremiumTotal = payroll.sssPremium_EE + payroll.sssPremium_ER;

        rem.philHealthPremium_EE = payroll.philHealthPremium_EE;
        rem.philHealthPremium_ER = payroll.philHealthPremium_ER;
        rem.philHealthPremiumTotal = payroll.philHealthPremium_EE + payroll.philHealthPremium_ER;

        rem.hdmfPremium_EE = payroll.hdmfPremium_EE;
        rem.hdmfPremium_ER = payroll.hdmfPremium_ER;
        rem.hdmfPremium_ER = payroll.hdmfPremium_ER;
        rem.hdmfPremiumTotal = payroll.hdmfPremium_EE + payroll.hdmfPremium_ER;

        rem.withHoldingTax = payroll.withHoldingTax;

        rem.sssSubmissionStatus = RemittanceStatus::PENDING;
        rem.phicSubmissionStatus = RemittanceStatus::PENDING;
        rem.hdmfSubmissionStatus = RemittanceStatus::PENDING;
        rem.withHoldingTaxSubmissionStatus = RemittanceStatus::PENDING;

        rem.dateCreated = Date::getTodayDate();  // Current date
        rem.dateModified = Date::getTodayDate(); // Current date
        v_Rem.push_back(rem);
    }

    // lastSubmittedDate remains default (1970-01-01)
    // submittedByUserId remains 0 (not yet submitted)
    this->remittanceRepo.insertRemittanceReports(v_Rem);
    return v_Rem.size() > 0;
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