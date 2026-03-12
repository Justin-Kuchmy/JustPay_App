#include "Services/GovernmentRemittanceService.h"

GovernmentRemittanceService::GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo) : remittanceRepo(remRepo), payrollRepo(payRepo)
{
}

sqlite3_int64 GovernmentRemittanceService::addRemittance(const GovernmentRemittance &r)
{
    return 0;
};
std::vector<sqlite3_int64> GovernmentRemittanceService::addRemittanceReports(const std::vector<GovernmentRemittance> &r)
{
    return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::generateFromPayroll(const std::vector<PayrollCalculationResults> *payrolls)
{
    return {};
};
std::optional<GovernmentRemittance> GovernmentRemittanceService::getById(int id)
{
    return std::nullopt;
};
std::optional<GovernmentRemittance> GovernmentRemittanceService::getByPayrollId(int payrollId)
{
    return std::nullopt;
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getByPeriod(const std::string &payPeriodText, std::optional<int> payPeriodHalf)
{
    return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getByEmployee(const std::string &employeeId)
{
    return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getPending()
{
    return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getAll()
{
    return {};
};
GovernmentRemittanceRepository::MonthlySummary getMonthlySummary(const std::string &monthYear)
{
    return {};
};
GovernmentRemittanceService::PeriodStatusSummary GovernmentRemittanceService::getPeriodStatusSummary(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
};
GovernmentRemittanceService::PeriodTotals GovernmentRemittanceService::getPeriodTotals(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
};
double GovernmentRemittanceService::getTotalEmployerContribution(const std::vector<GovernmentRemittance> &remittances)
{
    return 0.0;
};
double GovernmentRemittanceService::getTotalEmployeeContribution(const std::vector<GovernmentRemittance> &remittances)
{
    return 0.0;
};
double GovernmentRemittanceService::getTotalRemittance(const std::vector<GovernmentRemittance> &remittances)
{
    return 0.0;
};
bool GovernmentRemittanceService::submit(int remittanceId, const std::string &remittanceType, int submittedByUserId, const Date &submissionDate)
{
    return false;
};
bool GovernmentRemittanceService::confirm(int remittanceId, const std::string &remittanceType)
{
    return false;
};
bool GovernmentRemittanceService::reject(int remittanceId, const std::string &remittanceType)
{
    return false;
};
bool GovernmentRemittanceService::update(const GovernmentRemittance &remittance)
{
    return false;
};
bool GovernmentRemittanceService::remove(int id)
{
    return false;
};