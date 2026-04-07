#include <algorithm>
#include "include/Generated/ui_premium_values_widget.h"
#include "UI/PayrollComputation/Premium_Values_Widget.h"

PremiumValuesWidget::PremiumValuesWidget(std::vector<PayrollCalculationResults> *dataBus, QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PremiumValuesWidget), dataBus(dataBus)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &PremiumValuesWidget::applyClicked);
    auto conf = AppContext::instance().payrollService().loadConfig();
    if (conf)
        config = *conf;
}
PremiumValuesWidget::~PremiumValuesWidget()
{
    delete ui;
}

void PremiumValuesWidget::applyClicked()
{
    LOG_DEBUG("Apply Clicked");
    emit valuesApplied();
    applyValues();
}

void PremiumValuesWidget::applyValues()
{
    const int byWeekly = 2;
    auto truncateForCurrency = [](double v)
    {
        return std::stof(std::format("{:.2f}", v));
    };

    for (size_t i{}; i < dataBus->size(); i++)
    {
        auto &emp = dataBus->at(i);
        emp.dateProcessed = ui->dateProcessedEdit->date();
        emp.payPeriodText = emp.dateProcessed.toMonthYearString();
        emp.payPeriodHalf = ui->payrollPeriodComboBox->currentIndex() == 0 ? 1 : 2;
        std::vector<LoanLedger> empLoans = AppContext::instance().loanLedgerService().getAllLoanLedgers(emp.employeeId);
        std::vector<AttendanceLog> aLogs = AppContext::instance().attendanceLogService().getAllAttendanceLogsById(emp.employeeId);

        bool isFirstHalf = (emp.payPeriodHalf == 1);
        for (size_t j{}; j < empLoans.size(); j++)
        {
            LoanLedger currLoan = empLoans[j];
            LOG_DEBUG(currLoan.deductionsPerPayroll << " " << currLoan.deductionFirstHalf << " " << currLoan.deductionSecondHalf);

            if (isFirstHalf && currLoan.deductionFirstHalf)
                emp.loanDeductionsPerPayroll += currLoan.deductionsPerPayroll;

            if (!isFirstHalf && currLoan.deductionSecondHalf)
                emp.loanDeductionsPerPayroll += currLoan.deductionsPerPayroll;
        }

        emp.monthlyBasicSalary /= byWeekly;
        emp.monthlyAllowances /= byWeekly;
        emp.grossIncome = (emp.monthlyBasicSalary) + emp.overTimePay + (emp.monthlyAllowances);
        constexpr double MonthsPerYear = 12.0;
        constexpr double WorkingDaysPerYear = 314.0;
        constexpr double HoursPerDay = 8.0;

        const double hourlyRate = (emp.monthlyBasicSalary * 2) * MonthsPerYear / WorkingDaysPerYear / HoursPerDay;
        double totalOPpay{0.0};
        for (auto &item : aLogs)
        {

            int logDatePeriodHalf = (item.logDate.day <= 15) ? 1 : 2;
            if (logDatePeriodHalf == emp.payPeriodHalf)
            {
                totalOPpay += item.getRateMultiplier() * hourlyRate;
                LOG_DEBUG(item.to_string());
            }
        }
        emp.overTimePay = truncateForCurrency(totalOPpay);

        emp.applySSS = PayrollCalc::applies(config.sssSchedule, emp.payPeriodHalf);
        emp.sssPremium_EE = truncateForCurrency(PayrollCalc::calcSSS_EE(emp.monthlyBasicSalary, emp.applySSS));

        emp.applyPhilHealth = PayrollCalc::applies(config.philHealthSchedule, emp.payPeriodHalf);
        emp.philHealthPremium_EE = truncateForCurrency(PayrollCalc::calcPhilHealth_EE(emp.monthlyBasicSalary, emp.applyPhilHealth));

        emp.applyHDMF = PayrollCalc::applies(config.hdmfSchedule, emp.payPeriodHalf);
        emp.hdmfPremium_EE = truncateForCurrency(PayrollCalc::calcHDMF_EE(emp.monthlyBasicSalary, emp.applyHDMF));

        double tax = PayrollCalc::calcWithholding(PayrollCalc::calcTaxableIncome(emp));
        emp.withHoldingTax = truncateForCurrency(tax);

        emp.totalDeductions = truncateForCurrency(emp.sssPremium_EE + emp.philHealthPremium_EE + emp.hdmfPremium_EE + emp.withHoldingTax + emp.loanDeductionsPerPayroll);

        emp.netPay = truncateForCurrency(emp.monthlyBasicSalary + emp.overTimePay + emp.monthlyAllowances - emp.totalDeductions);

        emp.sssPremium_ER = truncateForCurrency(PayrollCalc::calcSSS_ER(emp.monthlyBasicSalary, emp.applySSS));

        emp.philHealthPremium_ER = truncateForCurrency(PayrollCalc::calcPhilHealth_ER(emp.monthlyBasicSalary, emp.applyPhilHealth));

        emp.hdmfPremium_ER = truncateForCurrency(PayrollCalc::calcHDMF_ER(emp.monthlyBasicSalary, emp.applyHDMF));
    }
}
