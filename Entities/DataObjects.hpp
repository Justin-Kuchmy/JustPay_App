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


};