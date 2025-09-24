#pragma once
#include <iostream>

class PayslipGenerationModule {
public:
    PayslipGenerationModule();
    ~PayslipGenerationModule();
    void selectPayrollPeriod();
    void selectEmployees();
};

class PayslipGenerator {
public:
    PayslipGenerator();
    ~PayslipGenerator();
    void previewPayslips();
    void exportToPDF();
    void exportToExcel();
    void emailPayslips();
};
