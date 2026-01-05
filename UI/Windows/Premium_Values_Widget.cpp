#include <algorithm>
#include "include/Generated/ui_premium_values_widget.h"
#include "UI/Premium_Values_Widget.h"


struct TaxBracket {
    double lowerBound;   // taxable income lower bound
    double baseTax;      // fixed base tax for bracket
    double rate;         // rate on excess
};
constexpr TaxBracket SEMI_MONTHLY_TAX_BRACKETS[] = {
    {0.0,       0.0,      0.0},      // 0-10,417
    {10417.01,  0.0,      0.15},     // 10,417.01 - 16,666
    {16667.0,   937.5,    0.20},     // 16,667 - 33,332
    {33333.0,   4270.7,   0.25},     // 33,333 - 83,332
    {83333.0,   16770.7,  0.30},     // 83,333 - 333,332
    {333333.0,  91770.7,  0.35}      // >333,333
};
constexpr size_t NUM_BRACKETS = sizeof(SEMI_MONTHLY_TAX_BRACKETS)/sizeof(TaxBracket);
constexpr long SSS_SALARY_CAP = 35000;
constexpr long PHIC_SALARY_FLOOR = 10000;
constexpr long PHIC_SALARY_CAP = 100000;
constexpr long HDMF_SALARY_CAP = 10000;
constexpr double SSS_EMP_RATE = 0.05;
constexpr double PHIC_EMP_RATE = 0.025;
constexpr double HDMF_EMP_RATE = 0.02;
PremiumValuesWidget::PremiumValuesWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::PremiumValuesWidget), dataBus(dataBus)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QPushButton::clicked, this, &PremiumValuesWidget::applyClicked);
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

inline double calcSSS(double gross, bool firstHalf) {
    if(!firstHalf) return 0.0;
    return std::min(gross, 
        static_cast<double>(SSS_SALARY_CAP)) * SSS_EMP_RATE;
}

inline double calcPhilHealth(double gross, bool firstHalf) {
    if(firstHalf) return 0.0;
    return std::clamp(gross, 
        static_cast<double>(PHIC_SALARY_FLOOR), 
        static_cast<double>(PHIC_SALARY_CAP)) * PHIC_EMP_RATE;
}


inline double calcHDMF(double gross, bool firstHalf) {
    if(firstHalf) return 0.0;
    return std::min(gross, 
        static_cast<double>(HDMF_SALARY_CAP)) * HDMF_EMP_RATE;
}

inline double calcTaxableIncome(PayrollCalculationResults &emp)
{
    double taxableIncome = emp.monthlyBasicSalary + emp.overTimePay - emp.sssPremium - emp.philHealthPremium - emp.hdmfPremium;
    return taxableIncome;

}

inline double calcWithholding(double taxableIncome)
{
    double withholdingTax = 0.0;
    for(size_t i{NUM_BRACKETS}; i > 0; --i)
    {
        if(taxableIncome < SEMI_MONTHLY_TAX_BRACKETS[i-1].lowerBound)
        {
            continue;
        }
        else
        {
            withholdingTax = 
                    SEMI_MONTHLY_TAX_BRACKETS[i-1].baseTax + 
                    SEMI_MONTHLY_TAX_BRACKETS[i-1].rate * 
                    (taxableIncome - SEMI_MONTHLY_TAX_BRACKETS[i-1].lowerBound);
            break;
        }
    }
    return withholdingTax;
}



void PremiumValuesWidget::applyValues()
{
    for(size_t i{}; i < dataBus->size(); i++)
    {
        auto& emp = dataBus->at(i);
        emp.deductionFirstHalf = ui->payrollPeriodComboBox->currentIndex() == 0;
        emp.deductionSecondHalf = ui->payrollPeriodComboBox->currentIndex() == 1;
        auto truncateForCurrency = [](double v) { return std::trunc(v * 100.0) / 100.0; };


        emp.grossIncome = emp.monthlyBasicSalary + emp.overTimePay + emp.monthlyAllowances;

        emp.sssPremium = truncateForCurrency(calcSSS(emp.monthlyBasicSalary, emp.deductionFirstHalf));

        emp.philHealthPremium = truncateForCurrency(calcPhilHealth(emp.monthlyBasicSalary, emp.deductionFirstHalf));

        emp.hdmfPremium = truncateForCurrency(calcHDMF(emp.monthlyBasicSalary, emp.deductionFirstHalf));

        emp.withHoldingTax = truncateForCurrency(calcWithholding(calcTaxableIncome(emp)));

        emp.totalDeductions = emp.sssPremium + emp.philHealthPremium + emp.hdmfPremium + emp.withHoldingTax + emp.loanDeductionsPerPayroll;

        emp.netPay = emp.monthlyBasicSalary + emp.overTimePay + emp.monthlyAllowances - emp.totalDeductions;

        LOG_DEBUG(emp.to_string());



    }
}

