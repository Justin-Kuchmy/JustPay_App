#ifndef DATAOBJECTS_HPP
#define DATAOBJECTS_HPP

#include <string>
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
    int day{};
    int month{};
    int year{};
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
    double monthlyBasicSalary{};
    double monthlyAllowances{};
    std::string personalEmail;
    bool isActive{true};
    Contact emergencyContact{};
    Dependent dependant{};


};

#include <string>
#include <cstdio>   // for snprintf, sscanf

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

// --- JobLevel ---
inline int to_int(JobLevel j) {
    return static_cast<int>(j);
}
inline JobLevel to_jobLevel(int i) {
    return static_cast<JobLevel>(i);
}

// --- EmploymentStatus ---
inline int to_int(EmploymentStatus s) {
    return static_cast<int>(s);
}
inline EmploymentStatus to_status(int i) {
    return static_cast<EmploymentStatus>(i);
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


#endif // DATAOBJECTS_HPP