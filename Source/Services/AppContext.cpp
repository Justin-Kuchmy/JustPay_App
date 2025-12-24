
#include "./Services/AppContext.h"
#include <QApplication>
#include <QDir>

sqlite3* openDb(const std::string& dbName) {
    sqlite3* db = nullptr;
    QString exeDir = QCoreApplication::applicationDirPath();
    QString dbPath = QDir(exeDir).filePath("../Resources/" + QString::fromStdString(dbName));
    const char* cpath = dbPath.toStdString().c_str();

    if (sqlite3_open(cpath, &db) != SQLITE_OK)
        throw std::runtime_error("Failed to open database");

    return db;
}

AppContext& AppContext::instance(const std::string& dbName) 
{
    static AppContext ctx(dbName);
    return ctx;
}

AppContext::AppContext(const std::string& dbName):
m_db(openDb(dbName)),
m_employeeRepo(m_db),
m_dependentRepo(m_db),
m_emergencyContactRepo(m_db),
m_loanLedgerRepo(m_db),
m_attendanceLogRepo(m_db),
m_employeeService(m_employeeRepo),
m_dependentService(m_dependentRepo),
m_emergencyContactService(m_emergencyContactRepo),
m_loanLedgerService(m_loanLedgerRepo),
m_attendanceLogService(m_attendanceLogRepo)
{
   
    m_employeeRepo.createTable();
    m_dependentRepo.createTable();
    m_emergencyContactRepo.createTable();
    m_loanLedgerRepo.createTable();
    m_attendanceLogRepo.createTable();
};

AppContext::~AppContext()
{
    std::cout << "\nAppContext destroyed!";
    if (m_db) {
        sqlite3_close(m_db);
        std::cout << "\nm_db closed";
    }

};



EmployeeService& AppContext::employeeService() noexcept
{
    return m_employeeService;
};

DependentService& AppContext::dependentService() noexcept
{
    return m_dependentService;
};

EmergencyContactService& AppContext::emergencyContactService() noexcept
{
    return m_emergencyContactService;
};

LoanLedgerService& AppContext::loanLedgerService() noexcept
{
    return m_loanLedgerService;
};

AttendanceLogService& AppContext::attendanceLogService() noexcept
{
    return m_attendanceLogService;
}
