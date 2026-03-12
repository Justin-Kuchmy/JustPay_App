#ifndef PAYROLLCALCULATIONS_H
#define PAYROLLCALCULATIONS_H
#include <algorithm>
#include "Models/DataObjects.h"
namespace PayrollCalc
{
    struct TaxBracket
    {
        double lowerBound; // taxable income lower bound
        double baseTax;    // fixed base tax for bracket
        double rate;       // rate on excess
    };
    constexpr TaxBracket SEMI_MONTHLY_TAX_BRACKETS[] = {
        {0.0, 0.0, 0.0},          // 0-10,417
        {10417.01, 0.0, 0.15},    // 10,417.01 - 16,666
        {16667.0, 937.5, 0.20},   // 16,667 - 33,332
        {33333.0, 4270.7, 0.25},  // 33,333 - 83,332
        {83333.0, 16770.7, 0.30}, // 83,333 - 333,332
        {333333.0, 91770.7, 0.35} // >333,333
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
    double calcTaxableIncome(PayrollCalculationResults &emp);
    double calcSSS_ER(double gross, bool applySSS);
    double calcPhilHealth_ER(double gross, bool applyPhilHealth);
    double calcHDMF_ER(double gross, bool applyHDMF);
    double calcWithholding(double taxableIncome);

    inline bool applies(DeductionSchedule schedule, int half)
    {
        return schedule == DeductionSchedule::BothHalves ||
               static_cast<int>(schedule) == half;
    }
}
#endif