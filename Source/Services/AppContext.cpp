
#include "./Services/AppContext.h"

AppContext& AppContext::instance(const std::string& dbName) 
{
    static AppContext ctx(dbName);
    return ctx;
}

AppContext::AppContext(const std::string& dbName)
{
    std::string path = "../Resources/" + dbName;
    const char* cpath = path.c_str();
    std::cout << "\nAppContext created! Trying to open " << cpath;
    if (sqlite3_open(cpath, &m_db) != SQLITE_OK)
        throw std::runtime_error("Failed to open database");

    m_employeeRepo = std::make_unique<EmployeeRepository>(m_db);
    m_dependentRepo = std::make_unique<DependentRepository>(m_db);
    m_emergencyContactRepo = std::make_unique<EmergencyContactRepository>(m_db);
    m_loanLedgerRepo = std::make_unique<LoanLedgerRepository>(m_db);

    m_employeeService = std::make_unique<EmployeeService>(*m_employeeRepo);
    m_dependentService = std::make_unique<DependentService>(*m_dependentRepo);
    m_emergencyContactService = std::make_unique<EmergencyContactService>(*m_emergencyContactRepo);
    m_loanLedgerService = std::make_unique<LoanLedgerService>(*m_loanLedgerRepo);
    
    m_employeeRepo->createTable();
    m_dependentRepo->createTable();
    m_emergencyContactRepo->createTable();
    m_loanLedgerRepo->createTable();
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



