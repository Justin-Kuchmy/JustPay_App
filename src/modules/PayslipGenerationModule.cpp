#include "../../include/modules/PayslipGenerationModule.hpp"
#include <iostream>

// PayslipGenerationModule
PayslipGenerationModule::PayslipGenerationModule() 
{
    std::cout << "PayslipGenerationModule created\n";
}
PayslipGenerationModule::~PayslipGenerationModule() 
{
    std::cout << "PayslipGenerationModule destroyed\n";
}
void PayslipGenerationModule::selectPayrollPeriod()
{
    std::cout << "PayslipGenerationModule::selectPayrollPeriod()" << std::endl;
};
void PayslipGenerationModule::selectEmployees()
{
    std::cout << "PayslipGenerationModule::selectEmployees()" << std::endl;
};

// PayslipGenerator
PayslipGenerator::PayslipGenerator() 
{
    std::cout << "PayslipGenerator created\n";
}
PayslipGenerator::~PayslipGenerator() 
{
    std::cout << "PayslipGenerator destroyed\n";
}
void PayslipGenerator::previewPayslips()
{
    std::cout << "PayslipGenerator::previewPayslips()" << std::endl;
};
void PayslipGenerator::exportToPDF()
{
    std::cout << "PayslipGenerator::exportToPDF()" << std::endl;
};
void PayslipGenerator::exportToExcel()
{
    std::cout << "PayslipGenerator::exportToExcel()" << std::endl;
};
void PayslipGenerator::emailPayslips()
{
    std::cout << "PayslipGenerator::emailPayslips()" << std::endl;
}
