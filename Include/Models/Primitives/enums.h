#pragma once
#include <string>
#define DEBUG_LOGS
#include "Utils/Log.h"

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

enum class AccountType
{
    Expense = 0,
    Liability = 1,
    Asset = 2
};
enum class EntryType
{
    Debit = 0,
    Credit = 1
};

enum class RemittanceStatus
{
    PENDING = 0,
    SUBMITTED = 1,
    CONFIRMED = 2,
    REJECTED = 3
};

enum class RemittanceType
{
    SSS,
    PHIC,
    HDMF
};

enum class DeductionSchedule
{
    FirstHalf = 1,
    SecondHalf = 2,
    BothHalves = 3
};

inline std::string loantype_to_string(int i)
{
    switch (static_cast<LoanType>(i))
    {
    case LoanType::HDMF_Salary_Loan:
        return "HDMF_Salary_Loan";
    case LoanType::HDMF_Housing_Loan:
        return "HDMF_Housing_Loan";
    case LoanType::HDMF_Calamity_Loan:
        return "HDMF_Calamity_Loan";
    case LoanType::SSS_Salary_Loan:
        return "SSS_Salary_Loan";
    case LoanType::SSS_Calamity_Loan:
        return "SSS_Calamity_Loan";
    case LoanType::Personal_Cash_Advance:
        return "Personal_Cash_Advance";
    case LoanType::Other:
        return "Other";
    default:
        return "unknown";
    }
}

// --- Department ---

inline std::string department_to_string(int i)
{
    switch (static_cast<Department>(i))
    {
    case Department::HR:
        return "HR";
    case Department::Finance:
        return "Finance";
    case Department::IT:
        return "IT";
    case Department::Operations:
        return "Operations";
    case Department::Sales:
        return "Sales";
    case Department::Marketing:
        return "Marketing";
    case Department::Admin:
        return "Admin";
    case Department::Legal:
        return "Legal";
    case Department::Engineering:
        return "Engineering";
    default:
        return "Unknown";
    }
}

// --- JobLevel ---

inline std::string joblevel_to_string(int i)
{
    switch (static_cast<JobLevel>(i))
    {
    case JobLevel::RankAndFile:
        return "RankAndFile";
    case JobLevel::Supervisory:
        return "Supervisory";
    case JobLevel::Managerial:
        return "Managerial";
    case JobLevel::Executive:
        return "Executive";
    case JobLevel::BoardOwnership:
        return "BoardOwnership";
    default:
        return "Unknown";
    }
}

// --- EmploymentStatus ---
inline std::string status_to_string(int i)
{
    switch (static_cast<EmploymentStatus>(i))
    {
    case EmploymentStatus::Regular:
        return "Regular";
    case EmploymentStatus::Probationary:
        return "Probationary";
    case EmploymentStatus::Contractual:
        return "Contractual";
    case EmploymentStatus::ProjectBased:
        return "ProjectBased";
    case EmploymentStatus::Seasonal:
        return "Seasonal";
    case EmploymentStatus::Casual:
        return "Casual";
    case EmploymentStatus::PartTime:
        return "PartTime";
    case EmploymentStatus::InternOJT:
        return "InternOJT";
    case EmploymentStatus::Consultant:
        return "Consultant";
    default:
        return "Unknown";
    }
}

inline std::string AccountType_to_string(int i)
{
    switch (static_cast<AccountType>(i))
    {
    case AccountType::Expense:
        return "Expense";
    case AccountType::Liability:
        return "Liability";
    case AccountType::Asset:
        return "Asset";
    default:
        return "Unknown";
    }
}

inline std::string EntryType_to_string(int i)
{
    switch (static_cast<EntryType>(i))
    {
    case EntryType::Debit:
        return "Debit";
    case EntryType::Credit:
        return "Credit";
    default:
        return "Unknown";
    }
}

inline std::string RemittanceStatus_to_string(RemittanceStatus status)
{
    switch (status)
    {
    case RemittanceStatus::CONFIRMED:
        return "Confirmed";
    case RemittanceStatus::PENDING:
        return "Pending";
    case RemittanceStatus::REJECTED:
        return "Rejected";
    case RemittanceStatus::SUBMITTED:
        return "Submitted";
    default:
        return "Unknown";
    }
}
inline std::string RemittanceType_to_string(RemittanceType type)
{
    switch (type)
    {
    case RemittanceType::SSS:
        return "SSS";
    case RemittanceType::PHIC:
        return "PHIC";
    case RemittanceType::HDMF:
        return "HDMF";
    default:
        return "Unknown";
    }
}

inline std::string month_to_text_string(int month)
{
    switch (month)
    {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    default:
        return "unknown";
    }
}
