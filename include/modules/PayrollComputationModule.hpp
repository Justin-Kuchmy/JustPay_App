#pragma once
#include <iostream>

class PayrollComputationModule {
public:
    PayrollComputationModule();
    ~PayrollComputationModule();
    void setPayrollPeriod();
    void computePayroll();
    void reviewComputedPayroll();
};

class PayrollCalculator {
public:
    PayrollCalculator();
    ~PayrollCalculator();
    void calculateOvertimePay();
    void applyAttendanceDeductions();
    void calculateTaxableIncome();
};
