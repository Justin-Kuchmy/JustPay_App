#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <memory>
#include "Services/EmployeeService.h"
#include "Services/DependentService.h"
#include "Services/EmergencyContactService.h"
#include "Repositories/EmployeeRepository.h"
#include "Repositories/DependentRepository.h"
#include "Repositories/EmergencyContactRepository.h"
#include <sqlite3.h>

class AppContext 
{
public:
    static AppContext& instance(const std::string& dbName = "payroll.db");
  
    EmployeeRepository& employeeRepo();
    DependentRepository& dependentRepo();
    EmergencyContactRepository& emergencyContactRepo();
    EmployeeService& employeeService();
    DependentService& dependentService();
    EmergencyContactService& emergencyContactService();
private: 
    sqlite3* m_db = nullptr;
    std::unique_ptr<EmployeeRepository> m_employeeRepo;
    std::unique_ptr<DependentRepository> m_dependentRepo;
    std::unique_ptr<EmergencyContactRepository> m_emergencyContactRepo;
    std::unique_ptr<EmployeeService> m_employeeService;
    std::unique_ptr<DependentService> m_dependentService;
    std::unique_ptr<EmergencyContactService> m_emergencyContactService;


    explicit AppContext(const std::string& dbName);
    ~AppContext() noexcept;
    
    AppContext(const AppContext&) = delete; //cannot be copied or assigned
    AppContext& operator=(const AppContext&) = delete; //cant assign or reassign
};
#endif