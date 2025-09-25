#include "../../include/modules/PayrollComputationModule.hpp"
#include <iostream>

// PayrollComputationModule
PayrollComputationModule::PayrollComputationModule() 
{
    std::cout << "PayrollComputationModule created\n";
}
PayrollComputationModule::PayrollComputationModule(ActionDispatcher& dispatcher) 
{
    std::cout << "PayrollComputationModule created\n";
    dispatcher.regAction("payroll-compute", "set_period", std::bind(&PayrollComputationModule::setPayrollPeriod, this));
    dispatcher.regAction("payroll-compute", "compute_payroll",  std::bind(&PayrollComputationModule::computePayroll, this));
    dispatcher.regAction("payroll-compute", "review_payroll", std::bind(&PayrollComputationModule::reviewComputedPayroll, this));
}
PayrollComputationModule::~PayrollComputationModule() 
{
    std::cout << "PayrollComputationModule destroyed\n";
}
void PayrollComputationModule::setPayrollPeriod()
{
    std::cout << "PayrollComputationModule::setPayrollPeriod()" << std::endl;
};
void PayrollComputationModule::computePayroll()
{
    std::cout << "PayrollComputationModule::computePayroll()" << std::endl;
};
void PayrollComputationModule::reviewComputedPayroll()
{
    std::cout << "PayrollComputationModule::reviewComputedPayroll()" << std::endl;
};

// Payroll
PayrollCalculator::PayrollCalculator() 
{
    std::cout << "Payroll created\n";
}
PayrollCalculator::PayrollCalculator(ActionDispatcher& dispatcher) 
{
    std::cout << "Payroll created\n";
    dispatcher.regAction("payroll-formula", "set_ot_rates",  std::bind(&PayrollCalculator::calculateOvertimePay , this));
    dispatcher.regAction("payroll-formula", "set_deductions", std::bind(&PayrollCalculator::applyAttendanceDeductions , this));
    dispatcher.regAction("payroll-formula", "set_taxable_formula",  std::bind(&PayrollCalculator::calculateTaxableIncome , this));
}
PayrollCalculator::~PayrollCalculator() 
{
    std::cout << "Payroll destroyed\n";
}
void PayrollCalculator::calculateOvertimePay()
{
    std::cout << "PayrollCalculator::calculateOvertimePay()" << std::endl;
};
void PayrollCalculator::applyAttendanceDeductions()
{
    std::cout << "PayrollCalculator::applyAttendanceDeductions()" << std::endl;
};
void PayrollCalculator::calculateTaxableIncome()
{
    std::cout << "PayrollCalculator::calculateTaxableIncome()" << std::endl;
};
