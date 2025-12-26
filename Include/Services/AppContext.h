#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <memory>
#include "Services/EmployeeService.h"
#include "Services/DependentService.h"
#include "Services/EmergencyContactService.h"
#include "Services/LoanLedgerService.h"
#include "Services/AttendanceLogService.h"
#include "Repositories/EmployeeRepository.h"
#include "Repositories/DependentRepository.h"
#include "Repositories/EmergencyContactRepository.h"
#include "Repositories/LoanLedgerRepository.h"
#include "Repositories/AttendanceLogRepository.h"
#include <sqlite3.h>

class EmployeeRepository;
class DependentRepository;
class EmergencyContactRepository;
class LoanLedgerRepository;
class AttendanceLogRepository;

class EmployeeService;
class DependentService;
class EmergencyContactService;
class LoanLedgerService;
class AttendanceLogService;

class AppContext 
{
public:
    static AppContext& instance(const std::string& dbName = "payroll.db");
  

    EmployeeService& employeeService() noexcept;
    DependentService& dependentService() noexcept;
    EmergencyContactService& emergencyContactService() noexcept;
    LoanLedgerService& loanLedgerService() noexcept;
    AttendanceLogService& attendanceLogService() noexcept;
private: 
    sqlite3* m_db = nullptr;
    EmployeeRepository m_employeeRepo;
    DependentRepository m_dependentRepo;
    EmergencyContactRepository m_emergencyContactRepo;
    LoanLedgerRepository m_loanLedgerRepo;
    AttendanceLogRepository m_attendanceLogRepo;

    EmployeeService m_employeeService;
    DependentService m_dependentService;
    EmergencyContactService m_emergencyContactService;
    LoanLedgerService m_loanLedgerService;
    AttendanceLogService m_attendanceLogService;


    explicit AppContext(const std::string& dbName);
    ~AppContext() noexcept;
    
    AppContext(const AppContext&) = delete; 
    AppContext& operator=(const AppContext&) = delete;
};
#endif