#include "Services/PayrollService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include <algorithm>
#include <QDate>

PayrollService::PayrollService(PayrollRepository &r) : repo(r)
{
}

// CREATE
sqlite3_int64 PayrollService::addPayroll(const PayrollCalculationResults &Payroll)
{
    return this->repo.insertPayroll(Payroll);
}

// READ
std::optional<PayrollCalculationResults> PayrollService::getPayrollByID(int id)
{
    return this->repo.getById(id);
}

std::vector<PayrollCalculationResults> PayrollService::getAllPayrolls()
{
    return this->repo.getAll();
}

std::vector<PayrollCalculationResults> PayrollService::getAllPayrollsAggregatedToMonthly()
{
    std::vector<PayrollCalculationResults> data = this->repo.getAll();
    std::vector<PayrollCalculationResults> monthlyVec{};

    std::sort(data.begin(), data.end(),
              [](const PayrollCalculationResults &a, const PayrollCalculationResults &b)
              {
                  if (a.employeeId != b.employeeId)
                      return a.employeeId < b.employeeId; // sort by employee ID first

                  // same employee → sort by month/year
                  QDate da = QDate::fromString(QString::fromStdString(a.payPeriodText), "MMMM yyyy");
                  QDate db = QDate::fromString(QString::fromStdString(b.payPeriodText), "MMMM yyyy");

                  return da < db;
              });

    for (size_t i{0}; i < data.size(); i += 2)
    {
        auto &firstHalfObj = data.at(i);
        auto &secondHalfObj = data.at(i + 1);
        PayrollCalculationResults combined{firstHalfObj};
        if (firstHalfObj.employeeId == secondHalfObj.employeeId &&
            firstHalfObj.payPeriodText == secondHalfObj.payPeriodText)
        {
            combined.employeeId = firstHalfObj.employeeId;
            combined.payPeriodText = firstHalfObj.payPeriodText;
            combined.monthlyBasicSalary = firstHalfObj.monthlyBasicSalary + secondHalfObj.monthlyBasicSalary;
            combined.monthlyAllowances = firstHalfObj.monthlyAllowances + secondHalfObj.monthlyAllowances;
            combined.overTimePay = firstHalfObj.overTimePay + secondHalfObj.overTimePay;
            combined.adjustments = firstHalfObj.adjustments + secondHalfObj.adjustments;
            combined.grossIncome = firstHalfObj.grossIncome + secondHalfObj.grossIncome;
            combined.sssPremium_EE = firstHalfObj.sssPremium_EE + secondHalfObj.sssPremium_EE;
            combined.philHealthPremium_EE = firstHalfObj.philHealthPremium_EE + secondHalfObj.philHealthPremium_EE;
            combined.hdmfPremium_EE = firstHalfObj.hdmfPremium_EE + secondHalfObj.hdmfPremium_EE;
            combined.loanDeductionsPerPayroll = firstHalfObj.loanDeductionsPerPayroll + secondHalfObj.loanDeductionsPerPayroll;
            combined.withHoldingTax = firstHalfObj.withHoldingTax + secondHalfObj.withHoldingTax;
            combined.totalDeductions = firstHalfObj.totalDeductions + secondHalfObj.totalDeductions;
            combined.netPay = firstHalfObj.netPay + secondHalfObj.netPay;
            combined.sssPremium_ER = firstHalfObj.sssPremium_ER + secondHalfObj.sssPremium_ER;
            combined.philHealthPremium_ER = firstHalfObj.philHealthPremium_ER + secondHalfObj.philHealthPremium_ER;
            combined.hdmfPremium_ER = firstHalfObj.hdmfPremium_ER + secondHalfObj.hdmfPremium_ER;
        }
        else
        {
            combined = firstHalfObj;
        }
        monthlyVec.push_back(combined);
    }

    return monthlyVec;
}

std::vector<PayrollCalculationResults> PayrollService::getAllPayrollsByEmployeeID(std::string &id)
{
    return this->repo.getAllById(id);
}

std::vector<PayrollCalculationResults> PayrollService::getPayrollByPeriod(const std::string &payPeriodText, std::optional<std::string> employeeId, std::optional<int> payPeriodHalf)
{
    return this->repo.getPayrollByPeriod(payPeriodText, employeeId, payPeriodHalf);
}

std::optional<PayrollConfig> PayrollService::loadConfig()
{
    return this->repo.loadConfig();
}

// UPDATE
bool PayrollService::updatePayroll(const PayrollCalculationResults &e)
{
    return this->repo.updatePayroll(e);
}

bool PayrollService::saveConfig(const PayrollConfig &config)
{
    auto res = this->repo.saveConfig(config);
    return res;
}

// DELETE
bool PayrollService::deletePayroll(int id)
{
    return this->repo.deletePayroll(id);
}
