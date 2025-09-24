#include "../../include/modules/EmployeeManagementModule.hpp"
#include <iostream>

// EmployeeManagementModule
EmployeeManagementModule::EmployeeManagementModule() 
{
    std::cout << "EmployeeManagementModule created\n";
}
EmployeeManagementModule::~EmployeeManagementModule() 
{
    std::cout << "EmployeeManagementModule destroyed\n";
}
void EmployeeManagementModule::addEmployee()
{
    std::cout << "EmployeeManagementModule::addEmployee()" << std::endl;
};
void EmployeeManagementModule::editEmployee()
{
    std::cout << "EmployeeManagementModule::editEmployee()" << std::endl;
};
void EmployeeManagementModule::deleteEmployee()
{
    std::cout << "EmployeeManagementModule::deleteEmployee()" << std::endl;
};
void EmployeeManagementModule::updateEmployeeStatus()
{
    std::cout << "EmployeeManagementModule::updateEmployeeStatus()" << std::endl;
};
void EmployeeManagementModule::updateEmergencyContact()
{
    std::cout << "EmployeeManagementModule::updateEmergencyContact()" << std::endl;
};

// LoanLedger
LoanLedger::LoanLedger() 
{
    std::cout << "LoanLedger created\n";
}
LoanLedger::~LoanLedger() 
{
    std::cout << "LoanLedger destroyed\n";
}
void LoanLedger::recordHDMFSalaryLoan()
{
    std::cout << "LoanLedger::recordHDMFSalaryLoan()" << std::endl;
};
void LoanLedger::recordHDMFHousingLoan()
{
    std::cout << "LoanLedger::recordHDMFHousingLoan()" << std::endl;
};
void LoanLedger::recordHDMFCalamityLoan()
{
    std::cout << "LoanLedger::recordHDMFCalamityLoan()" << std::endl;
};
void LoanLedger::recordSSSSalaryLoan()
{
    std::cout << "LoanLedger::recordSSSSalaryLoan()" << std::endl;
};
void LoanLedger::recordSSSCalamityLoan()
{
    std::cout << "LoanLedger::recordSSSCalamityLoan()" << std::endl;
};
void LoanLedger::recordPersonalCashAdvance()
{
    std::cout << "LoanLedger::recordPersonalCashAdvance()" << std::endl;
};
void LoanLedger::recordOtherLoan()
{
    std::cout << "LoanLedger::recordOtherLoan()" << std::endl;
};

// EmployeeAttendance
EmployeeAttendance::EmployeeAttendance() 
{
    std::cout << "EmployeeAttendance created\n";
}
EmployeeAttendance::~EmployeeAttendance() 
{
    std::cout << "EmployeeAttendance destroyed\n";
}
void EmployeeAttendance::uploadTimeLogsFromCSV()
{
    std::cout << " EmployeeAttendance::uploadTimeLogsFromCSV()" << std::endl;
};
void EmployeeAttendance::uploadTimeLogsFromAPI()
{
    std::cout << "EmployeeAttendance::uploadTimeLogsFromAPI()" << std::endl;
};
void EmployeeAttendance::editOvertime()
{
    std::cout << "EmployeeAttendance::editOvertime()" << std::endl;
};
void EmployeeAttendance::editTardiness()
{
    std::cout << "EmployeeAttendance::editTardiness()" << std::endl;
};
void EmployeeAttendance::editAbsences()
{
    std::cout << "EmployeeAttendance::editAbsences()" << std::endl;
};
void EmployeeAttendance::editUndertime()
{
    std::cout << "EmployeeAttendance::editUndertime()" << std::endl;
};
