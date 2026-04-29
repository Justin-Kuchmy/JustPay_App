#include "Services/GovernmentRemittanceService.h"

GovernmentRemittanceService::GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo) : remittanceRepo(remRepo), payrollRepo(payRepo)
{
}

sqlite3_int64 GovernmentRemittanceService::addRemittance(const GovernmentRemittance &r)
{
    return remittanceRepo.insertRemittance(r);
};
std::vector<sqlite3_int64> GovernmentRemittanceService::addRemittanceReports(const std::vector<GovernmentRemittance> &r)
{
    return remittanceRepo.insertRemittanceReports(r);
};
GovernmentRemittance GovernmentRemittanceService::createFromPayroll(const PayrollCalculationResults &payroll, RemittanceType type)
{
    GovernmentRemittance remittance{};
    remittance.payrollCalculationResultsId = payroll.id;
    remittance.employeeId = payroll.employeeId;
    remittance.fullName = payroll.fullName;
    remittance.contrib_Type = type;
    remittance.employeeDepartment = payroll.employeeDepartment;
    remittance.payPeriodDate = payroll.payPeriodDate;
    remittance.payPeriodHalf = payroll.payPeriodHalf;
    remittance.withHoldingTax = payroll.withHoldingTax;
    remittance.lastSubmittedDate = Date::getTodayDate();
    remittance.dateModified = Date::getTodayDate();
    remittance.submissionStatus = RemittanceStatus::PENDING;
    switch (type)
    {
    case RemittanceType::SSS:
        remittance.employee_Contrib = payroll.sssPremium_EE;
        remittance.employer_Contrib = payroll.sssPremium_ER;
        remittance.total_Contrib = payroll.sssPremium_EE + payroll.sssPremium_ER;
        break;
    case RemittanceType::PHIC:
        remittance.employee_Contrib = payroll.philHealthPremium_EE;
        remittance.employer_Contrib = payroll.philHealthPremium_ER;
        remittance.total_Contrib = payroll.philHealthPremium_EE + payroll.philHealthPremium_ER;
        break;
    case RemittanceType::HDMF:
        remittance.employee_Contrib = payroll.hdmfPremium_EE;
        remittance.employer_Contrib = payroll.hdmfPremium_ER;
        remittance.total_Contrib = payroll.hdmfPremium_EE + payroll.hdmfPremium_ER;
        break;
    }
    return remittance;
}
std::vector<GovernmentRemittance> GovernmentRemittanceService::createFromPayroll(const std::vector<PayrollCalculationResults> &payrolls)
{
    std::vector<GovernmentRemittance> vGr{};
    for (const PayrollCalculationResults &roll : payrolls)
    {
        switch (roll.payPeriodHalf)
        {
        case 1:
            vGr.push_back(createFromPayroll(roll, RemittanceType::SSS));
            break;
        case 2:
            vGr.push_back(createFromPayroll(roll, RemittanceType::PHIC));
            vGr.push_back(createFromPayroll(roll, RemittanceType::HDMF));
            break;
        }
    }
    return vGr;
};
std::optional<GovernmentRemittance> GovernmentRemittanceService::getById(int id)
{
    return remittanceRepo.getRemittanceById(id);
};
std::optional<GovernmentRemittance> GovernmentRemittanceService::getByPayrollId(int payrollId)
{
    return remittanceRepo.getRemittanceByPayrollId(payrollId);
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getByPeriod(const std::string &payPeriodDate, std::optional<int> payPeriodHalf)
{
    return remittanceRepo.getRemittancesByPeriod(payPeriodDate, payPeriodHalf);
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getByEmployee(const std::string &employeeId)
{
    return remittanceRepo.getRemittancesByEmployee(employeeId);
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getPending()
{
    return remittanceRepo.getRemittancesByStatus(RemittanceStatus::PENDING);
};
std::vector<GovernmentRemittance> GovernmentRemittanceService::getAll()
{
    return remittanceRepo.getAllRemittances();
};
auto GovernmentRemittanceService::getMonthlySummary(const std::string &monthYear) -> MonthlySummary
{
    auto items = remittanceRepo.getRemittancesByPeriod(monthYear);
    MonthlySummary summary{};
    summary.monthYear = monthYear;
    for (GovernmentRemittance &item : items)
    {
        switch (static_cast<int>(item.contrib_Type))
        {
        case 0:
            summary.totalSSSEE += item.employee_Contrib;
            summary.totalSSSER += item.employer_Contrib;
            break;
        case 1:
            summary.totalPHICEE += item.employee_Contrib;
            summary.totalPHICER += item.employer_Contrib;
            break;
        case 2:
            summary.totalHDMFEE += item.employee_Contrib;
            summary.totalHDMFER += item.employer_Contrib;
            break;
        }
        summary.totalWithholdingTax += item.withHoldingTax;
    }
    return summary;
};
auto GovernmentRemittanceService::getPeriodStatusSummary(const std::string &payPeriodDate) -> PeriodStatusSummary
{
    auto items = remittanceRepo.getRemittancesByPeriod(payPeriodDate);
    PeriodStatusSummary pss{};
    pss.payPeriodDate = payPeriodDate;
    for (auto &item : items)
    {
        switch (item.contrib_Type)
        {
        case RemittanceType::SSS:
            pss.sssRemittancesPending += (item.submissionStatus == RemittanceStatus::PENDING) ? 1 : 0;
            pss.sssRemittancesSubmitted += (item.submissionStatus == RemittanceStatus::SUBMITTED) ? 1 : 0;
            pss.sssRemittancesConfirmed += (item.submissionStatus == RemittanceStatus::CONFIRMED) ? 1 : 0;
            break;
        case RemittanceType::PHIC:
            pss.phicRemittancesPending += (item.submissionStatus == RemittanceStatus::PENDING) ? 1 : 0;
            pss.phicRemittancesSubmitted += (item.submissionStatus == RemittanceStatus::SUBMITTED) ? 1 : 0;
            pss.phicRemittancesConfirmed += (item.submissionStatus == RemittanceStatus::CONFIRMED) ? 1 : 0;
            break;
        case RemittanceType::HDMF:
            pss.hdmfRemittancesPending += (item.submissionStatus == RemittanceStatus::PENDING) ? 1 : 0;
            pss.hdmfRemittancesSubmitted += (item.submissionStatus == RemittanceStatus::SUBMITTED) ? 1 : 0;
            pss.hdmfRemittancesConfirmed += (item.submissionStatus == RemittanceStatus::CONFIRMED) ? 1 : 0;
            break;
        }
    }
    return pss;
};
auto GovernmentRemittanceService::getPeriodTotals(const std::string &payPeriodDate, std::optional<int> payPeriodHalf) -> PeriodTotals
{
    auto items = remittanceRepo.getRemittancesByPeriod(payPeriodDate, payPeriodHalf);
    PeriodTotals pt{};
    if (payPeriodHalf.has_value())
        pt.payPeriodHalf = *payPeriodHalf;
    pt.payPeriodDate = payPeriodDate;
    std::unordered_set<std::string> uniqueEmployees;
    for (auto &item : items)
        uniqueEmployees.insert(item.employeeId);
    pt.employeeCount = static_cast<int>(uniqueEmployees.size());
    for (auto &item : items)
    {
        pt.totalSSSEE += item.contrib_Type == RemittanceType::SSS ? item.employee_Contrib : 0.0;
        pt.totalSSSER += item.contrib_Type == RemittanceType::SSS ? item.employer_Contrib : 0.0;
        pt.totalPHICEE += item.contrib_Type == RemittanceType::PHIC ? item.employee_Contrib : 0.0;
        pt.totalPHICER += item.contrib_Type == RemittanceType::PHIC ? item.employer_Contrib : 0.0;
        pt.totalHDMFEE += item.contrib_Type == RemittanceType::HDMF ? item.employee_Contrib : 0.0;
        pt.totalHDMFER += item.contrib_Type == RemittanceType::HDMF ? item.employer_Contrib : 0.0;
        pt.totalWithholdingTax += item.withHoldingTax;
    }
    return pt;
};
double GovernmentRemittanceService::getTotalEmployerContribution(const std::vector<GovernmentRemittance> &remittances)
{
    double sum{0};
    for (auto &remit : remittances)
    {
        sum += remit.employer_Contrib;
    }
    return sum;
};
double GovernmentRemittanceService::getTotalEmployeeContribution(const std::vector<GovernmentRemittance> &remittances)
{
    double sum{0};
    for (auto &remit : remittances)
    {
        sum += remit.employee_Contrib;
    }
    return sum;
};
double GovernmentRemittanceService::getTotalRemittance(const std::vector<GovernmentRemittance> &remittances)
{
    return getTotalEmployeeContribution(remittances) + getTotalEmployerContribution(remittances);
};
bool GovernmentRemittanceService::submit(int remittanceId, const RemittanceType &remittanceType)
{
    return remittanceRepo.markAsSubmitted(remittanceId, remittanceType);
};
bool GovernmentRemittanceService::confirm(int remittanceId, const RemittanceType &remittanceType)
{
    return remittanceRepo.markAsConfirmed(remittanceId, remittanceType);
};
bool GovernmentRemittanceService::reject(int remittanceId, const RemittanceType &remittanceType)
{
    return remittanceRepo.markAsRejected(remittanceId, remittanceType);
};
bool GovernmentRemittanceService::update(const GovernmentRemittance &remittance)
{
    return remittanceRepo.updateRemittance(remittance);
};
bool GovernmentRemittanceService::remove(int id)
{
    return remittanceRepo.deleteRemittance(id);
};
