#pragma once
#include "Models/Primitives/enums.h"
#include "Models/Primitives/date.h"
#include <string>
#define DEBUG_LOGS
#include "Utils/Log.h"

struct Contact
{
    int contactId{};
    std::string name{};
    std::string relation{};
    std::string address{};
    std::string contactNo{};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "contactId: " << contactId
            << "\n name: " << name
            << "\n relation: " << relation
            << "\n address: " << address
            << "\n contactNo: " << contactNo;

        return oss.str();
    }
};

struct LoanLedger
{
    int loanLedgerId{};
    std::string employeeId{};
    LoanType loanType{};
    double principalAmount{};
    Date loanDate{};
    int NumOfAmortizations{};
    double deductionsPerPayroll{};
    bool deductionFirstHalf{};
    bool deductionSecondHalf{};
    bool status{};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "LoanLedger { "
            << "loanLedgerId: " << loanLedgerId
            << ", employeeId: " << employeeId
            << ", loanType: " << loantype_to_string(static_cast<int>(loanType))
            << ", principalAmount: " << principalAmount
            << ", loanDate: " << loanDate.to_string()
            << ", NumOfAmortizations: " << NumOfAmortizations
            << ", deductionsPerPayroll: " << deductionsPerPayroll
            << ", deductionFirstHalf: " << deductionFirstHalf
            << ", deductionSecondHalf: " << deductionSecondHalf
            << ", status: " << status
            << " }";
        return oss.str();
    }
};
struct Dependent
{
    int dependentId{};
    std::string name{};
    std::string relation{};
    Date birthday{};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "dependentId: " << dependentId
            << "\n name: " << name
            << "\n relation: " << relation
            << "\n birthday: " << birthday.to_string();

        return oss.str();
    }
};

struct EmployeeLeaveBalance
{
    std::string employeeId{""};
    int year{1970};

    double totalLeaveEarned{0.0};
    double leaveUsed{0.0};

    double unusedLeaveDays{0.0};
};

struct Employee
{
    std::string fullName{};
    std::string employeeId{};
    Department department{};
    std::string position{};
    JobLevel jobLevel{};
    EmploymentStatus status{};
    Date dateHired{};
    Date dateSeparation{};
    std::string sssNumber{};
    std::string philHealthNumber{};
    std::string hdmfNumber{};
    std::string tin{};
    std::string bankAccountNumber{};
    std::string clockInTimeStr{};
    std::string clockOutTimeStr{};
    double monthlyBasicSalary{};
    double monthlyAllowances{};
    std::string personalEmail{};
    std::string personalMobileNumber{};
    bool isActive{true};
    int contactId{};
    int dependentId{};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "Employee { "
            << "fullName: " << fullName
            << ", employeeId: " << employeeId
            << ", department: " << department_to_string(department)
            << ", position: " << position
            << ", jobLevel: " << joblevel_to_string(jobLevel)
            << ", status: " << status_to_string(status)
            << ", dateHired: " << dateHired.to_string()
            << ", dateSeparation: " << dateSeparation.to_string()
            << ", sssNumber: " << sssNumber
            << ", philHealthNumber: " << philHealthNumber
            << ", hdmfNumber: " << hdmfNumber
            << ", tin: " << tin
            << ", bankAccountNumber: " << bankAccountNumber
            << ", clockInTimeStr: " << clockInTimeStr
            << ", clockOutTimeStr: " << clockOutTimeStr
            << ", monthlyBasicSalary: " << monthlyBasicSalary
            << ", monthlyAllowances: " << monthlyAllowances
            << ", personalEmail: " << personalEmail
            << ", personalMobileNumber: " << personalMobileNumber
            << ", isActive: " << isActive
            << ", contactId: " << contactId
            << ", dependentId: " << dependentId
            << " }";
        return oss.str();
    };
};