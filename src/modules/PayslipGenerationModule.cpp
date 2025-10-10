#include "../../include/modules/PayslipGenerationModule.h"
#include <iostream>

// PayslipGenerationModule
PayslipGenerationModule::PayslipGenerationModule() 
{
}
PayslipGenerationModule::PayslipGenerationModule(ActionDispatcher& dispatcher) 
{
    dispatcher.regAction("payslip-generation", "select_payroll_period", std::bind(&PayslipGenerationModule::selectPayrollPeriod, this));
    dispatcher.regAction("payslip-generation", "select_employee",  std::bind(&PayslipGenerationModule::selectEmployees , this));
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
}
PayslipGenerator::PayslipGenerator(ActionDispatcher& dispatcher) 
{
    dispatcher.regAction("generate-payslips", "preview_payslips",  std::bind(&PayslipGenerator::previewPayslips, this));
    dispatcher.regAction("generate-payslips", "export_pdf",  std::bind(&PayslipGenerator::exportToPDF, this));
    dispatcher.regAction("generate-payslips", "export_excel",  std::bind(&PayslipGenerator::exportToExcel, this));
    dispatcher.regAction("generate-payslips", "email_payslips",  std::bind(&PayslipGenerator::emailPayslips, this));

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
