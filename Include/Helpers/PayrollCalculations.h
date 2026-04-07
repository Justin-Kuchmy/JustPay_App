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
        {0.0, 0.0, 0.0},          // 0-10417
        {10417.01, 0.0, 0.15},    // 10417.01 - 16666
        {16667.0, 937.5, 0.20},   // 16667 - 33332
        {33333.0, 4270.7, 0.25},  // 33333 - 83332
        {83333.0, 16770.7, 0.30}, // 83333 - 333332
        {333333.0, 91770.7, 0.35} // >333333
    };

    constexpr TaxBracket MONTHLY_TAX_BRACKETS[] = {
        {0.0, 0.0, 0.0},              // 0-20,832
        {20833.0, 0.0, 0.15},         // 20,833.01 - 33,332
        {33333.0, 1875.0, 0.20},      // 33,333 - 66,666
        {66667.0, 8541.83, 0.25},     // 66,667 - 166,666
        {166667.0, 33541.80, 0.30},   // 166,667 - 666,666
        {666667.0, 183541.80, 0.35}}; // > 666,667

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
        bool isBoth = (schedule == DeductionSchedule::BothHalves);
        int scheduledValue = static_cast<int>(schedule);
        bool validSingle = (scheduledValue == half);
        return isBoth || validSingle;
    }
}
#endif