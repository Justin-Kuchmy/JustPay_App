#ifndef APPCONTEXT_H
#define APPCONTEXT_H
#include <memory>
#include "Services/EmployeeService.h"
#include "Services/DependentService.h"
#include "Services/EmergencyContactService.h"
#include "Services/LoanLedgerService.h"
#include "Services/AttendanceLogService.h"
#include "Services/PayrollService.h"
#include "Services/JournalEntryService.h"
#include "Services/GovernmentRemittanceService.h"
#include "Services/YearEndBenefitsService.h"
#include "Services/LeaveBalanceService.h"
#include "Services/BudgetUtilService.h"
#include "Repositories/EmployeeRepository.h"
#include "Repositories/DependentRepository.h"
#include "Repositories/EmergencyContactRepository.h"
#include "Repositories/LoanLedgerRepository.h"
#include "Repositories/AttendanceLogRepository.h"
#include "Repositories/PayrollRepository.h"
#include "Repositories/JournalEntryRepository.h"
#include "Repositories/GovernmentRemittanceRepository.h"
#include "Repositories/LeaveRepository.h"
#include "Repositories/BudgetPeriodRepository.h"
#include "Repositories/DepartmentBudgetRepository.h"
#include <sqlite3.h>

class EmployeeRepository;
class DependentRepository;
class EmergencyContactRepository;
class LoanLedgerRepository;
class AttendanceLogRepository;
class PayrollRepository;
class JournalEntryRepository;
class GovernmentRemittanceRepository;
class LeaveRepository;
class BudgetPeriodRepository;
class DepartmentBudgetRepository;

class EmployeeService;
class DependentService;
class EmergencyContactService;
class LoanLedgerService;
class AttendanceLogService;
class PayrollService;
class JournalEntryService;
class GovernmentRemittanceService;
class YearEndBenefitsService;
class LeaveBalanceService;
class BudgetUtilService;

class AppContext
{
public:
    static AppContext &instance(const std::string &dbName = "payroll.db");
    static sqlite3 *openDb(const std::string &dbName);

    EmployeeService &employeeService() noexcept;
    DependentService &dependentService() noexcept;
    EmergencyContactService &emergencyContactService() noexcept;
    LoanLedgerService &loanLedgerService() noexcept;
    AttendanceLogService &attendanceLogService() noexcept;
    PayrollService &payrollService() noexcept;
    JournalEntryService &journalEntryService() noexcept;
    GovernmentRemittanceService &governmentRemittanceService() noexcept;
    YearEndBenefitsService &yearEndBenefitsService() noexcept;
    LeaveBalanceService &leaveBalanceService() noexcept;
    BudgetUtilService &budgetUtilService() noexcept;

private:
    sqlite3 *m_db = nullptr;
    EmployeeRepository m_employeeRepo;
    DependentRepository m_dependentRepo;
    EmergencyContactRepository m_emergencyContactRepo;
    LoanLedgerRepository m_loanLedgerRepo;
    AttendanceLogRepository m_attendanceLogRepo;
    PayrollRepository m_payrollRepo;
    JournalEntryRepository m_journalEntryRepo;
    GovernmentRemittanceRepository m_governmentRemittanceRepo;
    LeaveRepository m_leaveRepo;
    BudgetPeriodRepository m_budgetPeriodRepo;
    DepartmentBudgetRepository m_departmentBudgetRepo;

    EmployeeService m_employeeService;
    DependentService m_dependentService;
    EmergencyContactService m_emergencyContactService;
    LoanLedgerService m_loanLedgerService;
    AttendanceLogService m_attendanceLogService;
    PayrollService m_payrollService;
    JournalEntryService m_journalEntryService;
    GovernmentRemittanceService m_governmentRemittanceService;
    LeaveBalanceService m_leaveBalanceService;
    YearEndBenefitsService m_yearEndBenefitsService;
    BudgetUtilService m_budgetUtilService;

    explicit AppContext(const std::string &dbName);
    ~AppContext() noexcept;

    AppContext(const AppContext &) = delete;
    AppContext &operator=(const AppContext &) = delete;
};
#endif