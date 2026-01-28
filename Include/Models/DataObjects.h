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

struct EmailCrudentials
{
    std::string companyEmail{};
    std::string appPassword{};
};

struct Date
{
    int year{};
    int month{};
    int day{};

    static Date fromString(const std::string &s)
    {
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

        return Date{
            localTime.tm_year + 1900,
            localTime.tm_mon + 1,
            localTime.tm_mday};
    }

    bool operator>(const Date &rhs) const
    {

        return (year > rhs.year) ||
               (year == rhs.year && month > rhs.month) ||
               (year == rhs.year && month == rhs.month && day > rhs.day);
    }
};

struct Overtime
{
    double regular = 0.0;
    double rest_day = 0.0;
    double rest_day_plus = 0.0;
    double legal_holiday = 0.0;
    double legal_holiday_plus = 0.0;
    double special_holiday = 0.0;
    double special_holiday_plus = 0.0;
    double rest_plus_legal = 0.0;
    double rest_plus_special = 0.0;
    double night_shift_diff = 0.0;

    int sumMinutes() const
    {
        double sum = 0.0;
        sum += regular;
        sum += rest_day;
        sum += rest_day_plus;
        sum += legal_holiday;
        sum += legal_holiday_plus;
        sum += special_holiday;
        sum += special_holiday_plus;
        sum += rest_plus_legal;
        sum += rest_plus_special;
        sum += night_shift_diff;
        return static_cast<int>(sum);
    }

    double calculatePay() const
    {
        double sum = 0.0;
        sum += (regular / 60.0 * 1.25);
        sum += (rest_day / 60.0 * 1.30);
        sum += (rest_day_plus / 60.0 * 1.69);
        sum += (legal_holiday / 60.0 * 2.00);
        sum += (legal_holiday_plus / 60.0 * 2.60);
        sum += (special_holiday / 60.0 * 1.30);
        sum += (special_holiday_plus / 60.0 * 1.69);
        sum += (rest_plus_legal / 60.0 * 2.60);
        sum += (rest_plus_special / 60.0 * 3.38);
        sum += (night_shift_diff / 60.0 * 0.10);

        // this value is multiplied by the rate of pay
        return sum;
    }

    static double calculatePay(const std::string &json)
    {
        Overtime ot = fromJson(json);
        return ot.calculatePay();
    }

    static double extractNumber(const std::string &json, const std::string &key)
    {
        auto pos = json.find("\'" + key + "\'");
        if (pos == std::string::npos)
            return 0.0;

        pos = json.find(":", pos);
        if (pos == std::string::npos)
            return 0.0;

        std::stringstream ss(json.substr(pos + 1));
        double value = 0;
        ss >> value;
        return value;
    }

    static Overtime fromJson(const std::string &json)
    {
        Overtime ot;
        ot.regular = extractNumber(json, "regular");
        ot.rest_day = extractNumber(json, "rest_day");
        ot.rest_day_plus = extractNumber(json, "rest_day_plus");
        ot.legal_holiday = extractNumber(json, "legal_holiday");
        ot.legal_holiday_plus = extractNumber(json, "legal_holiday_plus");
        ot.special_holiday = extractNumber(json, "special_holiday");
        ot.special_holiday_plus = extractNumber(json, "special_holiday_plus");
        ot.rest_plus_legal = extractNumber(json, "rest_plus_legal");
        ot.rest_plus_special = extractNumber(json, "rest_plus_special");
        ot.night_shift_diff = extractNumber(json, "night_shift_diff");
        return ot;
    }

    std::string to_string() const
    {
        return std::format(
            "{{'regular':{},'rest_day':{},'rest_day_plus':{},"
            "'legal_holiday':{},'legal_holiday_plus':{},"
            "'special_holiday':{},'special_holiday_plus':{},"
            "'rest_plus_legal':{},'rest_plus_special':{},"
            "'night_shift_diff':{}}}",
            regular,
            rest_day,
            rest_day_plus,
            legal_holiday,
            legal_holiday_plus,
            special_holiday,
            special_holiday_plus,
            rest_plus_legal,
            rest_plus_special,
            night_shift_diff);
    }
};

struct AttendanceLog
{
    int logId{};
    std::string employeeId{};
    Date logDate{};
    int lateByMinute{};
    int underTimeByMinute{};
    int overTimeByMinute{};
    bool isAbsent{};
    std::string overtimeJson{};
    Overtime overtimeObj{};
    std::string notes{};

    double getOvertimePay() const
    {
        return overtimeObj.calculatePay();
    }

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "logId: " << logId
            << "\n employeeId: " << employeeId
            << "\n logDate: " << logDate.to_string()
            << "\n lateByMinute: " << lateByMinute
            << "\n underTimeByMinute: " << underTimeByMinute
            << "\n overTimeByMinute: " << overTimeByMinute
            << "\n isAbsent: " << ((isAbsent == 1) ? "True" : "False")
            << "\n Notes: " << notes;

        return oss.str();
    }
};

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

    // Instance method for debugging
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

class PayrollCalculationResults
{
public:
    int id{};
    std::string employeeId{""};
    std::string fullName{""};
    std::string employeeDepartment{""};
    std::string payPeriodText{"August 2025"};
    int payPeriodHalf{1};
    double monthlyBasicSalary{0.0};
    double monthlyAllowances{0.0};
    double overTimePay{0.0};
    double adjustments{0.0};
    double grossIncome{0.0};
    double sssPremium{0.0};
    double philHealthPremium{0.0};
    double hdmfPremium{0.0};
    double loanDeductionsPerPayroll{0.0};
    bool deductionFirstHalf{false};
    bool deductionSecondHalf{false};
    double withHoldingTax{0.0};
    double totalDeductions{0.0};
    double netPay{0.0};
    Date dateProcessed{Date(2025, 12, 18)};

    std::string to_string() const
    {
        std::ostringstream oss;
        oss << "PayrollCalculationResults { "
            << "\n dateProcessed: " << dateProcessed.to_string()
            << "\n employeeId: " << employeeId
            << "\n fullName: " << fullName
            << "\n employeeDepartment: " << employeeDepartment
            << "\n monthlyBasicSalary: " << monthlyBasicSalary
            << "\n monthlyAllowances: " << monthlyAllowances
            << "\n overTimePay: " << overTimePay
            << "\n adjustments: " << adjustments
            << "\n grossIncome: " << grossIncome
            << "\n sssPremium: " << sssPremium
            << "\n philHealthPremium: " << philHealthPremium
            << "\n hdmfPremium: " << hdmfPremium
            << "\n loanDeductionsPerPayroll: " << loanDeductionsPerPayroll
            << "\n deductionFirstHalf: " << deductionFirstHalf
            << "\n deductionSecondHalf: " << deductionSecondHalf
            << "\n totalDeductions: " << totalDeductions
            << "\n withHoldingTax: " << withHoldingTax
            << "\n netPay: " << netPay
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

#endif // DATAOBJECTS_HPP