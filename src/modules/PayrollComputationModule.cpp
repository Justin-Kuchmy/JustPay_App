#include "../../include/modules/PayrollComputationModule.hpp"
#include <iostream>

// PayrollComputationModule
PayrollComputationModule::PayrollComputationModule() 
{
    std::cout << "PayrollComputationModule created\n";
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
