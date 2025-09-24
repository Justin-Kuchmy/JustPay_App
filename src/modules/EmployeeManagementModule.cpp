#include "../../include/modules/EmployeeManagementModule.hpp"
#include <iostream>

// EmployeeManagementModule
EmployeeManagementModule::EmployeeManagementModule() 
{
    std::cout << "EmployeeManagementModule created\n";
}
EmployeeManagementModule::EmployeeManagementModule(ActionDispatcher& dispatcher) 
{
    std::cout << "EmployeeManagementModule created\n";
    dispatcher.regAction("employee-managment", "add_employee",  std::bind(&EmployeeManagementModule::addEmployee, this));
    dispatcher.regAction("employee-managment", "edit_employee", std::bind(&EmployeeManagementModule::editEmployee, this));
    dispatcher.regAction("employee-managment", "delete_employee", std::bind(&EmployeeManagementModule::deleteEmployee, this));
    dispatcher.regAction("employee-managment", "toggle_status", std::bind(&EmployeeManagementModule::updateEmployeeStatus, this));
    dispatcher.regAction("employee-managment", "manage_emergency",  std::bind(&EmployeeManagementModule::updateEmergencyContact, this));
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
LoanLedger::LoanLedger(ActionDispatcher& dispatcher) 
{
    std::cout << "LoanLedger created\n";
    dispatcher.regAction("loan-ledger", "hdmf_salary_loan", std::bind(&LoanLedger::recordHDMFSalaryLoan, this));
    dispatcher.regAction("loan-ledger", "hdmf_housing_loan",  std::bind(&LoanLedger::recordHDMFHousingLoan, this));
    dispatcher.regAction("loan-ledger", "hdmf_calamity_loan", std::bind(&LoanLedger::recordHDMFCalamityLoan, this));
    dispatcher.regAction("loan-ledger", "sss_salary_loan",  std::bind(&LoanLedger::recordSSSSalaryLoan, this));
    dispatcher.regAction("loan-ledger", "sss_calamity_loan",  std::bind(&LoanLedger::recordSSSCalamityLoan, this));
    dispatcher.regAction("loan-ledger", "cash_advance", std::bind(&LoanLedger::recordPersonalCashAdvance, this));
    dispatcher.regAction("loan-ledger", "other_loans", std::bind(&LoanLedger::recordOtherLoan, this));
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
EmployeeAttendance::EmployeeAttendance(ActionDispatcher& dispatcher) 
{
    std::cout << "EmployeeAttendance created\n";
    dispatcher.regAction("employee-attendance", "upload_time_logs", std::bind(&EmployeeAttendance::uploadTimeLogsFromCSV , this));
    dispatcher.regAction("employee-attendance", "upload_time_logs", std::bind(&EmployeeAttendance::uploadTimeLogsFromAPI, this));
    dispatcher.regAction("employee-attendance", "edit_overtime",  std::bind(&EmployeeAttendance::editOvertime, this));
    dispatcher.regAction("employee-attendance", "edit_tardiness", std::bind(&EmployeeAttendance::editTardiness, this));
    dispatcher.regAction("employee-attendance", "edit_absences",  std::bind(&EmployeeAttendance::editAbsences, this));
    dispatcher.regAction("employee-attendance", "edit_undertime", std::bind(&EmployeeAttendance::editUndertime, this));
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
