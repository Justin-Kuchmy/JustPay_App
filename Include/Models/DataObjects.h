#ifndef DATAOBJECTS_H
#define DATAOBJECTS_H

#include <string>
#include <iomanip>
#include <cstdio> 
#include <locale>
#include <QString>
#include <QVector>
#include <QDebug>

struct MenuOption {
    QString label;
    QString action;
    QString submenu;
};

struct MenuData {
    QString title;
    QVector<MenuOption> options;
};


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

struct Contact 
{
    std::string name;
    std::string relation;
    std::string address;
    std::string contactNo;
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
};
struct Dependent
{
    std::string name;
    std::string relation;
    Date birthday{};
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
    double monthlyBasicSalary{};
    double monthlyAllowances{};
    std::string personalEmail;
    bool isActive{true};
    Contact emergencyContact{};
    Dependent dependent{};
    friend std::ostream& operator<<(std::ostream& os, const Employee& p);
};



// =========================
// CONVERSION HELPERS
// =========================

// --- Department ---
inline int to_int(Department d) {
    return static_cast<int>(d);
}
inline Department to_department(int i) {
    return static_cast<Department>(i);
}

inline std::string department_to_string (int i) {
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
inline int to_int(JobLevel j) {
    return static_cast<int>(j);
}
inline JobLevel to_jobLevel(int i) {
    return static_cast<JobLevel>(i);
}
                      
                        
                        
                        
                        
inline std::string JobLevel_to_string (int i) {
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
inline int to_int(EmploymentStatus s) {
    return static_cast<int>(s);
}
inline EmploymentStatus to_status(int i) {
    return static_cast<EmploymentStatus>(i);
}

inline std::string Status_to_string(int i) {
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

// --- Date <-> String ---
inline std::string to_string(const Date& d) {
    char buffer[11]; // YYYY-MM-DD\0
    std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", d.year, d.month, d.day);
    return std::string(buffer);
}

inline Date from_string(const std::string& s) {
    Date d{};
    std::sscanf(s.c_str(), "%d-%d-%d", &d.year, &d.month, &d.day);
    return d;
}

inline std::ostream& operator<<(std::ostream& os, Employee& e)
{
    os.imbue(std::locale("en_PH.UTF-8"));
    os << std::fixed << std::setprecision(2);
    os << std::string(80,'=') << "\n";
    os << std::right << std::setw(32) << "" << "Employee Record" << std::setw(32) << "" << "\n";
    os << std::string(80,'=') << "\n";
    os <<std::left << std::setw(20) << "EmployeeId: " << std::setw(22) << e.employeeId                       << std::setw(25) << "| Monthly Basic Salary: " <<"₱"<< std::setw(15) << e.monthlyBasicSalary  << std::endl;
    os <<std::left << std::setw(20) << "Full Name: "  << std::setw(22) << e.fullName                         << std::setw(25) << "| Monthly Allowances: "   <<"₱"<< std::setw(15) << e.monthlyAllowances  << std::endl;
    os <<std::left << std::setw(20) << "Department: " << std::setw(22) << department_to_string(e.department) << std::setw(25) << "| Active: "               << std::setw(15) << e.isActive  << std::endl;
    os <<std::left << std::setw(20) << "JobLevel: "   << std::setw(22) << JobLevel_to_string(e.jobLevel)     << std::setw(25) << "|"<<std::endl;                 
    os <<std::left << std::setw(20) << "Position: "   << std::setw(22) << e.position                         << std::setw(25) << "|"<<std::endl;                 
    os <<std::left << std::setw(20) << "Status: "     << std::setw(22) << Status_to_string(e.status)         << std::setw(25) << "|"<<std::endl;                 
    os <<std::left << std::setw(20) << "Email: "      << std::setw(22) << e.personalEmail                    << std::setw(25) << "| SSS_Number: "           << std::setw(15) << e.sssNumber  << std::endl;
    os <<std::left << std::setw(20) << "Hired: "      << std::setw(22) << to_string(e.dateHired)             << std::setw(25) << "| PhilHealth_Number: "    << std::setw(15) << e.philHealthNumber  << std::endl;
    os <<std::left << std::setw(20) << "Separation: " << std::setw(22) << to_string(e.dateSeparation)        << std::setw(25) << "| HDMF_Number: "          << std::setw(15) << e.hdmfNumber  << std::endl;
    os << std::string(80,'=') << "\n";
    
    return os;
}

inline QDebug operator<<(QDebug debug, const Employee &e)
{
    std::ostringstream oss;
    oss.imbue(std::locale("en_PH.UTF-8"));
    oss << std::fixed << std::setprecision(2);
    oss << std::string(80, '=') << "\n";
    oss << std::right << std::setw(32) << "" << "Employee Record" << std::setw(32) << "" << "\n";
    oss << std::string(80, '=') << "\n";
    oss << std::left << std::setw(20) << "EmployeeId: " << std::setw(22) << e.employeeId<< std::setw(25)        << "| Monthly Basic Salary: "   << "₱" << std::setw(15)     << e.monthlyBasicSalary     << '\n';
    oss << std::left << std::setw(20) << "Full Name: "  << std::setw(22) << e.fullName<< std::setw(25)          << "| Monthly Allowances: "     << "₱" << std::setw(15)     << e.monthlyAllowances      << '\n';
    oss << std::left << std::setw(20) << "Department: " << std::setw(22) << department_to_string(e.department)  << std::setw(25)                << "| Active: "             << std::setw(15)            << e.isActive           << '\n';
    oss << std::left << std::setw(20) << "JobLevel: "   << std::setw(22) << JobLevel_to_string(e.jobLevel)      << std::setw(25)                << "|"                      << '\n';
    oss << std::left << std::setw(20) << "Position: "   << std::setw(22) << e.position<< std::setw(25)          << "|"                          << '\n';
    oss << std::left << std::setw(20) << "Status: "     << std::setw(22) << Status_to_string(e.status)          << std::setw(25)                << "| TIN: "                << std::setw(15)            << e.tin                << '\n';
    oss << std::left << std::setw(20) << "Email: "      << std::setw(22) << e.personalEmail                     << std::setw(25)                << "| SSS_Number: "         << std::setw(15)            << e.sssNumber          << '\n';
    oss << std::left << std::setw(20) << "Hired: "      << std::setw(22) << to_string(e.dateHired)              << std::setw(25)                << "| PhilHealth_Number: "  << std::setw(15)            << e.philHealthNumber   << '\n';
    oss << std::left << std::setw(20) << "Separation: " << std::setw(22) << to_string(e.dateSeparation)         << std::setw(25)                << "| HDMF_Number: "        << std::setw(15)            << e.hdmfNumber         << '\n';
    oss << std::left << std::setw(20) << "Contact: "    << std::setw(22) << e.emergencyContact.name             << std::setw(25)                << "| Dependent"            << std::setw(15)            << e.dependent.name     << '\n';
    oss << std::string(80, '=') << "\n";

    QDebugStateSaver saver(debug);
    debug.noquote().nospace() << QString::fromStdString(oss.str());
    return debug;
}
inline QDebug operator<<(QDebug debug, const Contact &e)
{
 std::ostringstream oss;
    oss.imbue(std::locale("en_PH.UTF-8"));
    oss << std::fixed << std::setprecision(2);
    oss << std::string(30, '=') << "\n";
    oss << std::right << std::setw(8) << "" << "Contact Record" << std::setw(9) << "" << "\n";
    oss << std::string(30, '=') << "\n";
    oss << std::left << std::setw(15) << "Name: " << std::setw(15) << e.name << "\n"; 
    oss << std::left << std::setw(15) << "Relation: "  << std::setw(15) << e.relation << "\n";       
    oss << std::left << std::setw(15) << "Contact Number: " << std::setw(15) << e.contactNo<< "\n"; 
    oss << std::left << std::setw(15) << "Address: "   << std::setw(15) << e.address<< "\n"; 
    oss << std::string(30, '=') << "\n";

    QDebugStateSaver saver(debug);
    debug.noquote().nospace() << QString::fromStdString(oss.str());
    return debug;
}

inline QDebug operator<<(QDebug debug, const Dependent &e)
{
 std::ostringstream oss;
    oss.imbue(std::locale("en_PH.UTF-8"));
    oss << std::fixed << std::setprecision(2);
    oss << std::string(30, '=') << "\n";
    oss << std::right << std::setw(7) << "" << "Dependent Record" << std::setw(7) << "" << "\n";
    oss << std::string(30, '=') << "\n";
    oss << std::left << std::setw(15) << "Name: " << std::setw(15)      << e.name<< "\n"; 
    oss << std::left << std::setw(15) << "Relation: "  << std::setw(15) << e.relation << "\n";         
    oss << std::left << std::setw(15) << "Birthday: " << std::setw(15)  << to_string(e.birthday)<< "\n"; 
    oss << std::string(30, '=') << "\n";

    QDebugStateSaver saver(debug);
    debug.noquote().nospace() << QString::fromStdString(oss.str());
    return debug;
}
#endif // DATAOBJECTS_HPP