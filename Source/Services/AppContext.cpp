
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
    qDebug() << "AppContext created! Trying to open " << cpath;
    if (sqlite3_open(cpath, &m_db) != SQLITE_OK)
        throw std::runtime_error("Failed to open database");

    m_employeeRepo = std::make_unique<EmployeeRepository>(m_db);
    m_employeeService = std::make_unique<EmployeeService>(*m_employeeRepo);
};

AppContext::~AppContext()
{
    qDebug() << "AppContext destroyed!";
    if (this->m_db) {
        sqlite3_close(this->m_db);
        qDebug() << "m_db closed";
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



