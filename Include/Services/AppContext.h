#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <memory>
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include <sqlite3.h>

class AppContext 
{
public:
    static AppContext& instance(const std::string& dbName = "payroll.db");
  
    EmployeeRepository& employeeRepo();
    EmployeeService& employeeService();
private: 
    sqlite3* m_db = nullptr;
    std::unique_ptr<EmployeeRepository> m_employeeRepo;
    std::unique_ptr<EmployeeService> m_employeeService;


    explicit AppContext(const std::string& dbName);
    ~AppContext() noexcept;
    
    AppContext(const AppContext&) = delete; //cannot be copied or assigned
    AppContext& operator=(const AppContext&) = delete; //cant assign or reassign
};
#endif