#pragma once
#include <iostream>
#include "../ActionDispatcher.hpp"

class PayslipGenerationModule {
public:
    PayslipGenerationModule();
    PayslipGenerationModule(ActionDispatcher& dispatcher);
    ~PayslipGenerationModule();
    void selectPayrollPeriod();
    void selectEmployees();
};

class PayslipGenerator {
public:
    PayslipGenerator();
    PayslipGenerator(ActionDispatcher& dispatcher);
    ~PayslipGenerator();
    void previewPayslips();
    void exportToPDF();
    void exportToExcel();
    void emailPayslips();
};
