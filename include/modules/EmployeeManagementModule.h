#pragma once
#include <iostream>
#include <string>
#include "../ActionDispatcher.h"

class EmployeeManagementModule {
public:
    EmployeeManagementModule();
    EmployeeManagementModule(ActionDispatcher& dispatcher);
    ~EmployeeManagementModule();
    void addEmployee();
    void editEmployee();
    void deleteEmployee();
    void updateEmployeeStatus();
    void updateEmergencyContact();
};

class LoanLedger {
public:
    LoanLedger();
    LoanLedger(ActionDispatcher& dispatcher);
    ~LoanLedger();
    void uploadLoanLedger();
    void viewLoanLedgersEmployee();
    void viewLoanLedgersType();
};

class EmployeeAttendance {
public:
    EmployeeAttendance();
    EmployeeAttendance(ActionDispatcher& dispatcher);
    ~EmployeeAttendance();
    void uploadTimeLogsFromCSV();
    void uploadTimeLogsFromAPI();
    void editOvertime();
    void editTardiness();
    void editAbsences();
    void editUndertime();
};
