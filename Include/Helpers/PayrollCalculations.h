#ifndef PAYROLLCALCULATIONS_H
#define PAYROLLCALCULATIONS_H
#include <algorithm>
#include "Models/Primitives/enums.h"
#include "Models/payroll.h"
namespace PayrollCalc
{
    struct TaxBracket
    {
        double lowerBound; // taxable income lower bound
        double baseTax;    // fixed base tax for bracket
        double rate;       // rate on excess
    };
    constexpr TaxBracket SEMI_MONTHLY_TAX_BRACKETS[] = {
        {0.0, 0.0, 0.0},            // 0 - 10,417
        {10417.01, 0.0, 0.15},      // 10,417.01 - 16,666
        {16667.0, 1250.0, 0.20},    // 16,667 - 33,332
        {33333.0, 5416.67, 0.25},   // 33,333 - 83,332
        {83333.0, 20416.67, 0.30},  // 83,333 - 333,332
        {333333.0, 100416.67, 0.35} // >333,333
    };

    constexpr TaxBracket MONTHLY_TAX_BRACKETS[] = {
        {0.0, 0.0, 0.0},            // 0 - 20,833
        {20833.0, 0.0, 0.15},       // 20,833 - 33,332
        {33333.0, 2500.0, 0.20},    // 33,333 - 66,666
        {66667.0, 10833.33, 0.25},  // 66,667 - 166,666
        {166667.0, 40833.33, 0.30}, // 166,667 - 666,666
        {666667.0, 200833.33, 0.35} // >666,667
    };

    constexpr size_t NUM_BRACKETS = sizeof(SEMI_MONTHLY_TAX_BRACKETS) / sizeof(TaxBracket);
    constexpr long SSS_SALARY_CAP = 35000;
    constexpr long PHIC_SALARY_FLOOR = 10000;
    constexpr long PHIC_SALARY_CAP = 100000;
    constexpr long HDMF_SALARY_CAP = 10000;
    constexpr double SSS_EMP_RATE = 0.05;
    constexpr double PHIC_EMP_RATE = 0.025;
    constexpr double HDMF_EMP_RATE = 0.02;

    constexpr double SSS_ER_RATE = 0.0795;
    constexpr double PHIC_ER_RATE = 0.025;
    constexpr double HDMF_ER_RATE = 0.02;

    double calcSSS_EE(double gross, bool applySSS);
    double calcPhilHealth_EE(double gross, bool applyPhilHealth);
    double calcHDMF_EE(double gross, bool applyHDMF);
    double calcTaxableIncome(const PayrollCalculationResults &emp);
    double calcSSS_ER(double gross, bool applySSS);
    double calcPhilHealth_ER(double gross, bool applyPhilHealth);
    double calcHDMF_ER(double gross, bool applyHDMF);
    double calcWithholding(double taxableIncome);

    inline float truncateForCurrency(double v)
    {
        return std::stof(std::format("{:.2f}", v));
    }
}
#endif