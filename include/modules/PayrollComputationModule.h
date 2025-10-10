#pragma once
#include <iostream>
#include "../ActionDispatcher.h"

class PayrollComputationModule {
public:
    PayrollComputationModule();
    PayrollComputationModule(ActionDispatcher& dispatcher);
    ~PayrollComputationModule();
    void setPayrollPeriod();
    void computePayroll();
    void reviewComputedPayroll();
};

class PayrollCalculator {
public:
    PayrollCalculator();
    PayrollCalculator(ActionDispatcher& dispatcher);
    ~PayrollCalculator();
    void calculateOvertimePay();
    void applyAttendanceDeductions();
    void calculateTaxableIncome();
};
