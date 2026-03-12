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
    { return std::trunc(v * 100.0) / 100.0; };

    for (size_t i{}; i < dataBus->size(); i++)
    {
        auto &emp = dataBus->at(i);
        std::vector<LoanLedger> empLoans = AppContext::instance().loanLedgerService().getAllLoanLedgers(emp.employeeId);
        emp.payPeriodHalf = ui->payrollPeriodComboBox->currentIndex() == 0 ? 1 : 2;

        for (size_t j{}; j < empLoans.size(); j++)
        {
            LOG_DEBUG(empLoans[j].deductionsPerPayroll << " " << empLoans[j].deductionFirstHalf << " " << empLoans[j].deductionSecondHalf);

            if (emp.payPeriodHalf - 1 && empLoans[j].deductionFirstHalf)
            {
                emp.loanDeductionsPerPayroll += empLoans[j].deductionsPerPayroll;
            }

            if (emp.payPeriodHalf - 1 && empLoans[j].deductionSecondHalf)
            {
                emp.loanDeductionsPerPayroll += empLoans[j].deductionsPerPayroll;
            }
        }

        emp.monthlyBasicSalary /= byWeekly;
        emp.monthlyAllowances /= byWeekly;
        emp.grossIncome = (emp.monthlyBasicSalary) + emp.overTimePay + (emp.monthlyAllowances);

        emp.applySSS = PayrollCalc::applies(config.sssSchedule, emp.payPeriodHalf);
        emp.sssPremium_EE = truncateForCurrency(PayrollCalc::calcSSS_EE(emp.monthlyBasicSalary, emp.applySSS));

        emp.applyPhilHealth = PayrollCalc::applies(config.philHealthSchedule, emp.payPeriodHalf);
        emp.philHealthPremium_EE = truncateForCurrency(PayrollCalc::calcPhilHealth_EE(emp.monthlyBasicSalary, emp.applyPhilHealth));

        emp.applyHDMF = PayrollCalc::applies(config.hdmfSchedule, emp.payPeriodHalf);
        emp.hdmfPremium_EE = truncateForCurrency(PayrollCalc::calcHDMF_EE(emp.monthlyBasicSalary, emp.applyHDMF));

        emp.withHoldingTax = truncateForCurrency(PayrollCalc::calcWithholding(PayrollCalc::calcTaxableIncome(emp)));

        emp.totalDeductions = emp.sssPremium_EE + emp.philHealthPremium_EE + emp.hdmfPremium_EE + emp.withHoldingTax + emp.loanDeductionsPerPayroll;

        emp.netPay = emp.monthlyBasicSalary + emp.overTimePay + emp.monthlyAllowances - emp.totalDeductions;

        emp.sssPremium_ER = truncateForCurrency(PayrollCalc::calcSSS_ER(emp.monthlyBasicSalary, emp.applySSS));

        emp.philHealthPremium_ER = truncateForCurrency(PayrollCalc::calcPhilHealth_ER(emp.monthlyBasicSalary, emp.applyPhilHealth));

        emp.hdmfPremium_ER = truncateForCurrency(PayrollCalc::calcHDMF_ER(emp.monthlyBasicSalary, emp.applyHDMF));

        LOG_DEBUG(emp.to_string());
    }
}
