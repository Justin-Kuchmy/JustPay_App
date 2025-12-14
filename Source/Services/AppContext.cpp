
#include "./Services/AppContext.h"
#include <QApplication>
#include <QDir>

AppContext& AppContext::instance(const std::string& dbName) 
{
    static AppContext ctx(dbName);
    return ctx;
}

AppContext::AppContext(const std::string& dbName)
{
   
    QString exeDir = QCoreApplication::applicationDirPath();
    QString dbPath = QDir(exeDir).filePath("../Resources/" + QString::fromStdString(dbName));
    const char* cpath = dbPath.toStdString().c_str();

    LOG_DEBUG("\nAppContext created! Trying to open " << cpath);
    if (sqlite3_open(cpath, &m_db) != SQLITE_OK)
        throw std::runtime_error("Failed to open database");

    m_employeeRepo = std::make_unique<EmployeeRepository>(m_db);
    m_dependentRepo = std::make_unique<DependentRepository>(m_db);
    m_emergencyContactRepo = std::make_unique<EmergencyContactRepository>(m_db);
    m_loanLedgerRepo = std::make_unique<LoanLedgerRepository>(m_db);
    m_attendanceLogRepo = std::make_unique<AttendanceLogRepository>(m_db);

    m_employeeService = std::make_unique<EmployeeService>(*m_employeeRepo);
    m_dependentService = std::make_unique<DependentService>(*m_dependentRepo);
    m_emergencyContactService = std::make_unique<EmergencyContactService>(*m_emergencyContactRepo);
    m_loanLedgerService = std::make_unique<LoanLedgerService>(*m_loanLedgerRepo);
    m_attendanceLogService = std::make_unique<AttendanceLogService>(*m_attendanceLogRepo);
    
    m_employeeRepo->createTable();
    m_dependentRepo->createTable();
    m_emergencyContactRepo->createTable();
    m_loanLedgerRepo->createTable();
    m_attendanceLogRepo->createTable();
};

AppContext::~AppContext()
{
    std::cout << "\nAppContext destroyed!";
    if (this->m_db) {
        sqlite3_close(this->m_db);
        std::cout << "\nm_db closed";
    }

};

EmployeeRepository& AppContext::employeeRepo()
{
     return *m_employeeRepo; 
};

EmployeeService& AppContext::employeeService()
{
    return *m_employeeService;
};
  
DependentRepository& AppContext::dependentRepo()
{
    return *m_dependentRepo;
};

LoanLedgerRepository& AppContext::loanLedgerRepo()
{
    return *m_loanLedgerRepo;
};

AttendanceLogRepository& AppContext::attendanceLogRepo()
{
    return *m_attendanceLogRepo;
}

DependentService& AppContext::dependentService()
{
    return *m_dependentService;
};

EmergencyContactRepository& AppContext::emergencyContactRepo()
{
    return *m_emergencyContactRepo;
};

EmergencyContactService& AppContext::emergencyContactService()
{
    return *m_emergencyContactService;
};

LoanLedgerService& AppContext::loanLedgerService()
{
    return *m_loanLedgerService;
};

AttendanceLogService& AppContext::attendanceLogService()
{
    return *m_attendanceLogService;
}
