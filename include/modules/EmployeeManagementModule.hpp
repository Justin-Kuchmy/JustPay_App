#pragma once
#include <iostream>
#include <string>

class EmployeeManagementModule {
public:
    EmployeeManagementModule();
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
    ~EmployeeAttendance();
    void uploadTimeLogsFromCSV();
    void uploadTimeLogsFromAPI();
    void editOvertime();
    void editTardiness();
    void editAbsences();
    void editUndertime();
};
