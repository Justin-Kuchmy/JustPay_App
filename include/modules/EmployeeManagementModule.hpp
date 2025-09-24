#pragma once
#include <iostream>
#include <string>
#include "../ActionDispatcher.hpp"

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
    void recordHDMFSalaryLoan();
    void recordHDMFHousingLoan();
    void recordHDMFCalamityLoan();
    void recordSSSSalaryLoan();
    void recordSSSCalamityLoan();
    void recordPersonalCashAdvance();
    void recordOtherLoan();
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
