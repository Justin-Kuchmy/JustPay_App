#ifndef DATAOBJECTS_H
#define DATAOBJECTS_H

#include <string>
#include <iomanip>
#include <cstdio> 
#include <locale>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iostream>
#define DEBUG_LOGS
#include "../Utils/Log.h"

enum Department
{
    HR,
    Finance,
    IT,
    Operations,
    Sales,       
    Marketing,   
    Admin,       
    Legal,       
    Engineering,

};
enum JobLevel 
{
    RankAndFile,
    Supervisory,
    Managerial,
    Executive,
    BoardOwnership
};
enum EmploymentStatus 
{
    Regular,       
    Probationary,  
    Contractual,   
    ProjectBased,  
    Seasonal,      
    Casual,        
    PartTime,      
    InternOJT,     
    Consultant,    
};
enum LoanType 
{
    HDMF_Salary_Loan,
    HDMF_Housing_Loan,
    HDMF_Calamity_Loan,
    SSS_Salary_Loan,
    SSS_Calamity_Loan,
    Personal_Cash_Advance,
    Other,     
};



// =========================
// CONVERSION ENUM DECLARATIONS
// =========================


 inline std::string loantype_to_string(int i);
 inline std::string department_to_string(int i);
 inline std::string joblevel_to_string(int i);
 inline std::string status_to_string(int i);

struct Contact 
{
    int contactId;
    std::string name;
    std::string relation;
    std::string address;
    std::string contactNo;

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

struct Date {
    int year{};
    int month{};
    int day{};
    
    static Date fromString(const std::string& s) {
        Date d;
        char dash;
        std::istringstream iss(s);
        iss >> d.year >> dash >> d.month >> dash >> d.day;
        return d;
    }

    std::string to_string() const 
    {
        std::ostringstream oss;
        oss << std::setw(4) << std::setfill('0') << year << '-'
            << std::setw(2) << std::setfill('0') << month << '-'
            << std::setw(2) << std::setfill('0') << day;
        return oss.str();
    }
    
    static Date getTodayDate()
    {
        
        const auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm localTime = *std::localtime(&t);
        
        return Date {
            localTime.tm_year+1900,
            localTime.tm_mon+1,
            localTime.tm_mday
        };
        
    }
    
    bool operator>(const Date& rhs) const
    {
        
        return 
        (year > rhs.year) || 
        (year == rhs.year && month > month) || 
        (year == rhs.year && month == month && day > rhs.day);
    }
};
struct LoanLedger
{
    int loanLedgerId;
    std::string employeeId;
    LoanType loanType;
    double principalAmount;
    Date loanDate;
    int NumOfAmortizations;
    double deductionsPerPayroll;
    bool deductionFirstHalf;
    bool deductionSecondHalf;
    bool status;

    std::string to_string() const {
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
    int dependentId;
    std::string name;
    std::string relation;
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
struct Employee 
{
    std::string fullName;
    std::string employeeId;
    Department department{};
    std::string position;
    JobLevel jobLevel{};
    EmploymentStatus status{};
    Date dateHired{};
    Date dateSeparation{};
    std::string sssNumber;
    std::string philHealthNumber;
    std::string hdmfNumber;
    std::string tin;
    std::string bankAccountNumber;
    double monthlyBasicSalary{};
    double monthlyAllowances{};
    std::string personalEmail;
    std::string personalMobileNumber;
    bool isActive{true};
    int contactId{};
    int dependentId{};

    // Instance method for debugging
    std::string to_string() const {
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



// =========================
// CONVERSION ENUM HELPERS
// =========================

// --- Loan Type ---

inline std::string loantype_to_string(int i)
{
    switch(static_cast<LoanType>(i))
    {
        case LoanType::HDMF_Salary_Loan:        return "HDMF_Salary_Loan";
        case LoanType::HDMF_Housing_Loan:       return "HDMF_Housing_Loan";
        case LoanType::HDMF_Calamity_Loan:      return "HDMF_Calamity_Loan";
        case LoanType::SSS_Salary_Loan:         return "SSS_Salary_Loan";
        case LoanType::SSS_Calamity_Loan:       return "SSS_Calamity_Loan";
        case LoanType::Personal_Cash_Advance:   return "Personal_Cash_Advance";
        case LoanType::Other:                   return "Other";
        default:                                return "unknown";
    }
}


// --- Department ---


inline std::string department_to_string(int i) {
    switch (static_cast<Department>(i)) {
        case Department::HR:            return "HR";                
        case Department::Finance:       return "Finance";                
        case Department::IT:            return "IT";               
        case Department::Operations:    return "Operations";             
        case Department::Sales:         return "Sales";                   
        case Department::Marketing:     return "Marketing";               
        case Department::Admin:         return "Admin";                   
        case Department::Legal:         return "Legal";                   
        case Department::Engineering:   return "Engineering";             
        default:                        return "Unknown";
    }
}

// --- JobLevel ---
                   
                        
inline std::string joblevel_to_string(int i) {
    switch (static_cast<JobLevel>(i)) {
        case JobLevel::RankAndFile:     return "RankAndFile";                
        case JobLevel::Supervisory:     return "Supervisory";                
        case JobLevel::Managerial:      return "Managerial";               
        case JobLevel::Executive:       return "Executive";             
        case JobLevel::BoardOwnership:  return "BoardOwnership";                      
        default:                        return "Unknown";
    }
}

// --- EmploymentStatus ---
inline std::string status_to_string(int i) {
    switch (static_cast<EmploymentStatus>(i)) {
        case EmploymentStatus::Regular:         return "Regular";
        case EmploymentStatus::Probationary:    return "Probationary";
        case EmploymentStatus::Contractual:     return "Contractual";
        case EmploymentStatus::ProjectBased:    return "ProjectBased";
        case EmploymentStatus::Seasonal:        return "Seasonal";
        case EmploymentStatus::Casual:          return "Casual";
        case EmploymentStatus::PartTime:        return "PartTime";
        case EmploymentStatus::InternOJT:       return "InternOJT";
        case EmploymentStatus::Consultant:      return "Consultant";
        default:                        return "Unknown";
    }
}

#endif // DATAOBJECTS_HPP