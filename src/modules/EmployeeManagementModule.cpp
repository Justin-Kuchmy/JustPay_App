#include "../../include/modules/EmployeeManagementModule.hpp"
#include <iostream>
#include "../../Services/EmployeeService.cpp"

// EmployeeManagementModule
EmployeeManagementModule::EmployeeManagementModule() 
{

}
EmployeeManagementModule::EmployeeManagementModule(ActionDispatcher& dispatcher) 
{
    EmployeeRepository* pRepo = new EmployeeRepository();
    EmployeeService employeeService(pRepo);
    pRepo->createTable();
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
}
LoanLedger::LoanLedger(ActionDispatcher& dispatcher) 
{
    dispatcher.regAction("loan-ledger", "upload_loan_ledger", std::bind(&LoanLedger::uploadLoanLedger, this));
    dispatcher.regAction("loan-ledger", "view_loan_ledgers_employee",  std::bind(&LoanLedger::viewLoanLedgersEmployee, this));
    dispatcher.regAction("loan-ledger", "view_loan_ledgers_type", std::bind(&LoanLedger::viewLoanLedgersType, this));
}
LoanLedger::~LoanLedger() 
{
    std::cout << "LoanLedger destroyed\n";
}
void LoanLedger::uploadLoanLedger()
{
    std::cout << "LoanLedger::uploadLoanLedger()" << std::endl;
};
void LoanLedger::viewLoanLedgersEmployee()
{
    std::cout << "LoanLedger::viewLoanLedgersEmployee()" << std::endl;
};
void LoanLedger::viewLoanLedgersType()
{
    std::cout << "LoanLedger::viewLoanLedgersType()" << std::endl;
};


// EmployeeAttendance
EmployeeAttendance::EmployeeAttendance() 
{
}
EmployeeAttendance::EmployeeAttendance(ActionDispatcher& dispatcher) 
{
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
