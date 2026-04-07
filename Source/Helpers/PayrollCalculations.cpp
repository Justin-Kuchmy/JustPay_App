#include "Helpers/PayrollCalculations.h"

namespace PayrollCalc
{
    double calcSSS_EE(double gross, bool applySSS)
    {
        if (!applySSS)
            return 0.0;
        return std::min(gross,
                        static_cast<double>(SSS_SALARY_CAP)) *
               SSS_EMP_RATE;
    }

    double calcPhilHealth_EE(double gross, bool applyPhilHealth)
    {
        if (!applyPhilHealth)
            return 0.0;
        return std::clamp(gross,
                          static_cast<double>(PHIC_SALARY_FLOOR),
                          static_cast<double>(PHIC_SALARY_CAP)) *
               PHIC_EMP_RATE;
    }

    double calcHDMF_EE(double gross, bool applyHDMF)
    {
        if (!applyHDMF)
            return 0.0;
        return std::min(gross,
                        static_cast<double>(HDMF_SALARY_CAP)) *
               HDMF_EMP_RATE;
    }

    double calcTaxableIncome(PayrollCalculationResults &emp)
    {
        double taxableIncome = emp.monthlyBasicSalary + emp.overTimePay - emp.sssPremium_EE - emp.philHealthPremium_EE - emp.hdmfPremium_EE;
        return taxableIncome;
    }

    double calcSSS_ER(double gross, bool applySSS)
    {
        if (!applySSS)
            return 0.0;
        double res = std::min(gross,
                              static_cast<double>(SSS_SALARY_CAP)) *
                     SSS_ER_RATE;
        return res;
    }

    double calcPhilHealth_ER(double gross, bool applyPhilHealth)
    {
        if (!applyPhilHealth)
            return 0.0;
        double res = std::clamp(gross,
                                static_cast<double>(PHIC_SALARY_FLOOR),
                                static_cast<double>(PHIC_SALARY_CAP)) *
                     PHIC_ER_RATE;
        return res;
    }

    double calcHDMF_ER(double gross, bool applyHDMF)
    {
        if (!applyHDMF)
            return 0.0;
        double res = std::min(gross,
                              static_cast<double>(HDMF_SALARY_CAP)) *
                     HDMF_ER_RATE;
        return res;
    }

    double calcWithholding(double taxableIncome)
    {
        double withholdingTax = 0.0;
        for (size_t i{NUM_BRACKETS}; i > 0; --i)
        {
            if (taxableIncome < SEMI_MONTHLY_TAX_BRACKETS[i - 1].lowerBound)
            {
                continue;
            }
            else
            {
                withholdingTax =
                    SEMI_MONTHLY_TAX_BRACKETS[i - 1].baseTax +
                    SEMI_MONTHLY_TAX_BRACKETS[i - 1].rate *
                        (taxableIncome - SEMI_MONTHLY_TAX_BRACKETS[i - 1].lowerBound);
                LOG_DEBUG("withholdingTax = " << withholdingTax);
                break;
            }
        }
        return withholdingTax;
    }

}